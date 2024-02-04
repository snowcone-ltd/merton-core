#include "prdp.h"

#include "device/rcp/rdp/rdp_core.h"
#include "device/rcp/mi/mi_controller.h"

#include "rdp_device.hpp"
#include "context.hpp"
#include "device.hpp"

extern "C" void core_log(const char *fmt, ...);
extern "C" void core_vlog(const char *fmt, va_list arg);

static GFX_INFO GFX;
static std::vector<RDP::RGBA> PIXELS;
static uint32_t CMD_DATA[0x10000];
static int CMD_PTR;
static int CMD_CUR;
static std::unique_ptr<RDP::CommandProcessor> CMDP;
static std::unique_ptr<Vulkan::Device> VDEVICE;
static std::unique_ptr<Vulkan::Context> VCONTEXT;

// Settings
static bool SYNC = true;

static uint8_t UPSCALING = 1;
static bool SUPER_SAMPLED_READ_BACK = false;
static bool SUPER_SAMPLED_DITHER = true;

static bool NATIVE_TEXTURE_LOD = false;
static bool NATIVE_TEX_RECT = true;

static uint32_t DOWNSCALE_STEPS = 0;
static bool DIVOT_FILTER = true;
static bool GAMMA_DITHER = true;
static bool VI_AA = true;
static bool VI_SCALE = true;
static bool DITHER_FILTER = true;
static bool INTERLACING = true;

static const uint8_t CMD_LEN_LUT[64] = {
	1, 1, 1, 1, 1, 1, 1, 1, 4, 6, 12, 14, 12, 14, 20, 22,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
};

static void (*VIDEO_FUNC)(void *, uint32_t, uint32_t, void *);
static void *VIDEO_OPAQUE;

class PRDP_Logger : public Util::LoggingInterface {
	bool log(const char *tag, const char *fmt, va_list va)
	{
		char level = tag[1];
		if (level == 'I')
			return true;

		size_t new_fmt_len = strlen(fmt) + 128;
		char *new_fmt = (char *) malloc(new_fmt_len);
		snprintf(new_fmt, new_fmt_len, "[parallel-rdp %c] %s", level, fmt);
		core_vlog(new_fmt, va);
		free(new_fmt);

		return true;
	}
};

static PRDP_Logger LOGGER;

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
		*APIVersion = 0x020500;

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

static void prdp_load_commands(uint32_t dpc_cur, uint32_t len, uint8_t *mem, uint32_t mask)
{
	uint32_t offset = dpc_cur & mask;

	for (uint32_t x = 0; x < len; x++) {
		CMD_DATA[2 * CMD_PTR + 0] = *((uint32_t *) (mem + offset));
		CMD_DATA[2 * CMD_PTR + 1] = *((uint32_t *) (mem + offset + 4));
		CMD_PTR++;

		offset += 8;
		offset &= mask;
	}
}

void PRDP_ProcessRDPList(void)
{
	uint32_t dpc_cur = *GFX.DPC_CURRENT_REG & 0xFFFFF8;
	uint32_t dpc_end = *GFX.DPC_END_REG & 0xFFFFF8;

	// Angrylion does not clear this bit here.
	// *GFX.DPC_STATUS_REG &= ~DPC_STATUS_FREEZE;

	if (dpc_cur >= dpc_end)
		return;

	uint32_t len = (dpc_end - dpc_cur) >> 3;

	if ((CMD_PTR + len) & ~(0x3FFFF >> 3))
		return;

	if (*GFX.DPC_STATUS_REG & DPC_STATUS_XBUS_DMEM_DMA) {
		prdp_load_commands(dpc_cur, len, GFX.DMEM, 0xFF8);

	} else {
		if (dpc_end > 0x7FFFFFF || dpc_cur > 0x7FFFFFF)
			return;

		prdp_load_commands(dpc_cur, len, GFX.RDRAM, 0xFFFFF8);
	}

	while (CMD_CUR < CMD_PTR) {
		uint32_t w1 = CMD_DATA[2 * CMD_CUR];
		uint32_t command = (w1 >> 24) & 0x3F;
		uint8_t cmd_length = CMD_LEN_LUT[command];

		if (CMD_PTR - CMD_CUR - cmd_length < 0) {
			*GFX.DPC_START_REG = *GFX.DPC_CURRENT_REG = *GFX.DPC_END_REG;
			return;
		}

		if (command >= 8 && CMDP)
			CMDP->enqueue_command(cmd_length * 2, &CMD_DATA[2 * CMD_CUR]);

		if (RDP::Op(command) == RDP::Op::SyncFull) {
			if (SYNC && CMDP)
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
	Util::set_thread_logging_interface(&LOGGER);

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

	if (!VCONTEXT->init_instance(instance_ext.data(), instance_ext.size(), 0))
		return 0;

	if (!VCONTEXT->init_device(VK_NULL_HANDLE, VK_NULL_HANDLE, device_ext.data(), device_ext.size(), 0))
		return 0;

	VDEVICE = std::make_unique<Vulkan::Device>();
	VDEVICE->set_context(*VCONTEXT);

	RDP::CommandProcessorFlags flags = 0;

	switch (UPSCALING) {
		case 2:
			flags |= RDP::COMMAND_PROCESSOR_FLAG_UPSCALING_2X_BIT;
			break;
		case 4:
			flags |= RDP::COMMAND_PROCESSOR_FLAG_UPSCALING_4X_BIT;
			break;
		case 8:
			flags |= RDP::COMMAND_PROCESSOR_FLAG_UPSCALING_8X_BIT;
			break;
	}

	if (UPSCALING > 1 && SUPER_SAMPLED_READ_BACK)
		flags |= RDP::COMMAND_PROCESSOR_FLAG_SUPER_SAMPLED_READ_BACK_BIT;

	if (SUPER_SAMPLED_DITHER)
		flags |= RDP::COMMAND_PROCESSOR_FLAG_SUPER_SAMPLED_DITHER_BIT;

	CMDP = std::make_unique<RDP::CommandProcessor>(
		*VDEVICE, GFX.RDRAM, 0, *GFX.RDRAM_SIZE, *GFX.RDRAM_SIZE / 2, flags);

	if (!CMDP->device_is_supported())
		return 0;

	RDP::Quirks quirks = {};
	quirks.set_native_texture_lod(NATIVE_TEXTURE_LOD);
	quirks.set_native_resolution_tex_rect(NATIVE_TEX_RECT);
	CMDP->set_quirks(quirks);

	return 1;
}

void PRDP_RomClosed(void)
{
	memset(&GFX, 0, sizeof(GFX));
	memset(CMD_DATA, 0, sizeof(CMD_DATA));
	CMD_PTR = 0;
	CMD_CUR = 0;

	PIXELS.clear();
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

	RDP::ScanoutOptions opts = {};
	opts.persist_frame_on_invalid_input = true;
	opts.vi.aa = VI_AA;
	opts.vi.scale = VI_SCALE;
	opts.vi.dither_filter = DITHER_FILTER;
	opts.vi.divot_filter = DIVOT_FILTER;
	opts.vi.gamma_dither = GAMMA_DITHER;
	opts.blend_previous_frame = INTERLACING;
	opts.upscale_deinterlacing = !INTERLACING;
	opts.downscale_steps = DOWNSCALE_STEPS;
	opts.crop_overscan_pixels = 0;

	uint32_t w = 0;
	uint32_t h = 0;
	CMDP->scanout_sync(PIXELS, w, h, opts);

	if (VIDEO_FUNC)
		VIDEO_FUNC(PIXELS.data(), w, h, VIDEO_OPAQUE);

	CMDP->begin_frame_context();
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
