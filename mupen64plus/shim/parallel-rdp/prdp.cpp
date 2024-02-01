#include "prdp.h"

#include "device/rcp/rdp/rdp_core.h"
#include "device/rcp/mi/mi_controller.h"

#include "rdp_device.hpp"
#include "context.hpp"
#include "device.hpp"

extern "C" void core_log(const char *fmt, ...);

static GFX_INFO GFX;
static std::vector<RDP::RGBA> PIXELS;
static uint32_t CMD_DATA[0x10000];
static int CMD_PTR;
static int CMD_CUR;
static bool SYNCH = true;
static std::unique_ptr<RDP::CommandProcessor> CMDP;
static std::unique_ptr<Vulkan::Device> VDEVICE;
static std::unique_ptr<Vulkan::Context> VCONTEXT;

static const unsigned CMD_LEN_LUT[64] = {
	1, 1, 1, 1, 1, 1, 1, 1, 4, 6, 12, 14, 12, 14, 20, 22,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
};

static void (*VIDEO_FUNC)(void *, uint32_t, uint32_t, void *);
static void *VIDEO_OPAQUE;

void prdp_set_video_func(void (*func)(void *, uint32_t, uint32_t, void *), void *opaque)
{
	VIDEO_FUNC = func;
	VIDEO_OPAQUE = opaque;
}

m64p_error PRDP_PluginStartup(m64p_dynlib_handle _CoreLibHandle, void *Context,
	void (*DebugCallback)(void *, int, const char *))
{
	return M64ERR_SUCCESS;
}

m64p_error PRDP_PluginShutdown(void)
{
	return M64ERR_SUCCESS;
}

m64p_error PRDP_PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion,
	int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
	if (PluginType)
		*PluginType = M64PLUGIN_GFX;

	if (PluginVersion)
		*PluginVersion = 0x010000;

	if (APIVersion)
		*APIVersion = 0x020100;

	if (PluginNamePtr)
		*PluginNamePtr = "paraLLEl-RDP";

	if (Capabilities)
		*Capabilities = 0;

	return M64ERR_SUCCESS;
}

int PRDP_InitiateGFX(GFX_INFO Gfx_Info)
{
	GFX = Gfx_Info;

	return 1;
}

void PRDP_MoveScreen(int xpos, int ypos)
{
}

void PRDP_ProcessDList(void)
{
}

void PRDP_ProcessRDPList(void)
{
	uint32_t DP_CURRENT = *GFX.DPC_CURRENT_REG & 0xFFFFF8;
	uint32_t DP_END = *GFX.DPC_END_REG & 0xFFFFF8;

	// Angrylion does not clear this bit here.
	// *GFX.DPC_STATUS_REG &= ~DPC_STATUS_FREEZE;

	int length = DP_END - DP_CURRENT;
	if (length <= 0)
		return;

	length = (unsigned) length >> 3;
	if ((CMD_PTR + length) & ~(0x3FFFF >> 3))
		return;

	uint32_t offset = DP_CURRENT;
	if (*GFX.DPC_STATUS_REG & DPC_STATUS_XBUS_DMEM_DMA) {
		do {
			offset &= 0xFF8;
			CMD_DATA[2 * CMD_PTR + 0] = *((uint32_t *) (GFX.DMEM + offset));
			CMD_DATA[2 * CMD_PTR + 1] = *((uint32_t *) (GFX.DMEM + offset + 4));
			offset += 8;
			CMD_PTR++;
		} while (--length > 0);

	} else {
		if (DP_END > 0x7FFFFFF || DP_CURRENT > 0x7FFFFFF) {
			return;

		} else {
			do {
				offset &= 0xFFFFF8;
				CMD_DATA[2 * CMD_PTR + 0] = *((uint32_t *) (GFX.RDRAM + offset));
				CMD_DATA[2 * CMD_PTR + 1] = *((uint32_t *) (GFX.RDRAM + offset + 4));
				offset += 8;
				CMD_PTR++;
			} while (--length > 0);
		}
	}

	while (CMD_CUR - CMD_PTR < 0) {
		uint32_t w1 = CMD_DATA[2 * CMD_CUR];
		uint32_t command = (w1 >> 24) & 63;
		int cmd_length = CMD_LEN_LUT[command];

		if (CMD_PTR - CMD_CUR - cmd_length < 0) {
			*GFX.DPC_START_REG = *GFX.DPC_CURRENT_REG = *GFX.DPC_END_REG;
			return;
		}

		if (command >= 8 && CMDP)
			CMDP->enqueue_command(cmd_length * 2, &CMD_DATA[2 * CMD_CUR]);

		if (RDP::Op(command) == RDP::Op::SyncFull) {
			if (SYNCH && CMDP)
				CMDP->wait_for_timeline(CMDP->signal_timeline());

			*GFX.MI_INTR_REG |= MI_INTR_DP;
			GFX.CheckInterrupts();
		}

		CMD_CUR += cmd_length;
	}

	CMD_PTR = 0;
	CMD_CUR = 0;
	*GFX.DPC_START_REG = *GFX.DPC_CURRENT_REG = *GFX.DPC_END_REG;
}

int PRDP_RomOpen(void)
{
	if (!Vulkan::Context::init_loader(nullptr))
		return 0;

	VkApplicationInfo info = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		NULL,
		"paraLLEl-RDP", 0,
		"paraLLEl-RDP", 0,
		VK_API_VERSION_1_1
	};

	std::vector<const char *> instance_ext = {};
	std::vector<const char *> device_ext = {};

	VCONTEXT = std::make_unique<Vulkan::Context>();
	VCONTEXT->set_application_info(&info);
	VCONTEXT->set_num_thread_indices(1);

	if (!VCONTEXT->init_instance(instance_ext.data(), instance_ext.size(),
		Vulkan::ContextCreationFlagBits::CONTEXT_CREATION_ENABLE_ADVANCED_WSI_BIT))
	{
		return 0;
	}

	if (!VCONTEXT->init_device(VK_NULL_HANDLE, VK_NULL_HANDLE, device_ext.data(), device_ext.size(),
		Vulkan::ContextCreationFlagBits::CONTEXT_CREATION_ENABLE_ADVANCED_WSI_BIT))
	{
		return 0;
	}

	VDEVICE = std::make_unique<Vulkan::Device>();
	VDEVICE->set_context(*VCONTEXT);

	CMDP = std::make_unique<RDP::CommandProcessor>(
		*VDEVICE, GFX.RDRAM, 0, *GFX.RDRAM_SIZE, *GFX.RDRAM_SIZE / 2, RDP::CommandProcessorFlags{});

	if (!CMDP->device_is_supported())
		return 0;

	RDP::Quirks quirks;
	quirks.set_native_texture_lod(false);
	quirks.set_native_resolution_tex_rect(true);
	CMDP->set_quirks(quirks);

	return 1;
}

void PRDP_RomClosed(void)
{
	memset(&GFX, 0, sizeof(GFX));
	memset(CMD_DATA, 0, sizeof(CMD_DATA));
	CMD_PTR = 0;
	CMD_CUR = 0;

	CMDP.reset();
	VDEVICE.reset();
	VCONTEXT.reset();
}

void PRDP_ShowCFB(void)
{
}

void PRDP_UpdateScreen(void)
{
	CMDP->signal_timeline();

	CMDP->set_vi_register(RDP::VIRegister::Control, *GFX.VI_STATUS_REG);
	CMDP->set_vi_register(RDP::VIRegister::Origin, *GFX.VI_ORIGIN_REG);
	CMDP->set_vi_register(RDP::VIRegister::Width, *GFX.VI_WIDTH_REG);
	CMDP->set_vi_register(RDP::VIRegister::Intr, *GFX.VI_INTR_REG);
	CMDP->set_vi_register(RDP::VIRegister::VCurrentLine, *GFX.VI_V_CURRENT_LINE_REG);
	CMDP->set_vi_register(RDP::VIRegister::Timing, *GFX.VI_V_BURST_REG);
	CMDP->set_vi_register(RDP::VIRegister::VSync, *GFX.VI_V_SYNC_REG);
	CMDP->set_vi_register(RDP::VIRegister::HSync, *GFX.VI_H_SYNC_REG);
	CMDP->set_vi_register(RDP::VIRegister::Leap, *GFX.VI_LEAP_REG);
	CMDP->set_vi_register(RDP::VIRegister::HStart, *GFX.VI_H_START_REG);
	CMDP->set_vi_register(RDP::VIRegister::VStart, *GFX.VI_V_START_REG);
	CMDP->set_vi_register(RDP::VIRegister::VBurst, *GFX.VI_V_BURST_REG);
	CMDP->set_vi_register(RDP::VIRegister::XScale, *GFX.VI_X_SCALE_REG);
	CMDP->set_vi_register(RDP::VIRegister::YScale, *GFX.VI_Y_SCALE_REG);

	RDP::ScanoutOptions opts;
	opts.persist_frame_on_invalid_input = true;
	opts.vi.aa = true;
	opts.vi.scale = true;
	opts.vi.dither_filter = true;
	opts.vi.divot_filter = true;
	opts.vi.gamma_dither = true;
	opts.blend_previous_frame = true;
	opts.upscale_deinterlacing = false;
	opts.downscale_steps = 0;
	opts.crop_overscan_pixels = 0;

	uint32_t w = 0;
	uint32_t h = 0;
	CMDP->scanout_sync(PIXELS, w, h, opts);

	CMDP->begin_frame_context();

	if (VIDEO_FUNC)
		VIDEO_FUNC(PIXELS.data(), w, h, VIDEO_OPAQUE);
}

void PRDP_ViStatusChanged(void)
{
}

void PRDP_ViWidthChanged(void)
{
}

void PRDP_ChangeWindow(void)
{
}

void PRDP_ReadScreen2(void *dest, int *width, int *height, int front)
{
}

void PRDP_SetRenderingCallback(void (*callback)(int))
{
}

void PRDP_ResizeVideoOutput(int width, int height)
{
}

void PRDP_FBWrite(unsigned int addr, unsigned int size)
{
}

void PRDP_FBRead(unsigned int addr)
{
}

void PRDP_FBGetFrameBufferInfo(void *pinfo)
{
}
