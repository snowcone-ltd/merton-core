#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <cmath>

#include <windows.h>

#include "rdp_device.hpp"
#include "context.hpp"
#include "device.hpp"
#include "logging.hpp"

#include "matoya.h"

constexpr unsigned SCANOUT_ORIGIN = 1024;
constexpr unsigned SCANOUT_WIDTH = 320;
constexpr unsigned SCANOUT_HEIGHT = 240;

struct context {
	MTY_App *app;
	MTY_Window window;
	uint32_t frame_count;
	void *rdram_ptr;
	Vulkan::Device *device;
	std::unique_ptr<RDP::CommandProcessor> processor;
	std::vector<RDP::RGBA> vec;
	bool quit;
};

static void setup_default_vi_registers(RDP::CommandProcessor &processor)
{
	processor.set_vi_register(RDP::VIRegister::Control, RDP::VI_CONTROL_AA_MODE_RESAMP_REPLICATE_BIT |
		RDP::VI_CONTROL_TYPE_RGBA8888_BIT);
	processor.set_vi_register(RDP::VIRegister::Origin, SCANOUT_ORIGIN);
	processor.set_vi_register(RDP::VIRegister::Width, SCANOUT_WIDTH);
	processor.set_vi_register(RDP::VIRegister::VSync, RDP::VI_V_SYNC_NTSC);
	processor.set_vi_register(RDP::VIRegister::XScale, RDP::make_vi_scale_register(512, 0));
	processor.set_vi_register(RDP::VIRegister::YScale, RDP::make_vi_scale_register(1024, 0));
	processor.set_vi_register(RDP::VIRegister::VStart, RDP::make_vi_start_register(RDP::VI_V_OFFSET_NTSC,
		RDP::VI_V_OFFSET_NTSC + 224 * 2));
	processor.set_vi_register(RDP::VIRegister::HStart, RDP::make_vi_start_register(RDP::VI_H_OFFSET_NTSC,
		RDP::VI_H_OFFSET_NTSC + 640));
}

static void render_frame(struct context *ctx, Vulkan::Device &device, RDP::CommandProcessor &processor)
{
	RDP::ScanoutOptions options = {};

	uint32_t w = 0;
	uint32_t h = 0;
	processor.scanout_sync(ctx->vec, w, h, options);

	MTY_RenderDesc desc = {};
	desc.format = MTY_COLOR_FORMAT_RGBA;
	desc.filter = MTY_FILTER_LINEAR;
	desc.effects[0] = MTY_EFFECT_SCANLINES;
	desc.levels[0] = 0.70f;
	desc.imageWidth = w;
	desc.imageHeight = h;
	desc.cropWidth = w;
	desc.cropHeight = h;

	MTY_WindowDrawQuad(ctx->app, ctx->window, ctx->vec.data(), &desc);
}

static void update_vram(void *vram_ptr, unsigned frame_index)
{
	auto *base = static_cast<uint32_t *>(vram_ptr) + SCANOUT_ORIGIN / 4;

	for (unsigned y = 0; y < SCANOUT_HEIGHT; y++) {
		for (unsigned x = 0; x < SCANOUT_WIDTH; x++) {
			// Generate a funky pattern.

			float r = std::sin(float(x) * 0.134f + float(y) * 0.234f + float(frame_index) * 0.05f);
			float g = std::sin(float(x) * 0.434f + float(y) * 0.234f + float(frame_index) * 0.02f);
			float b = std::sin(float(x) * -0.234f + float(y) * -0.234f + float(frame_index) * 0.03f);

			r = r * 0.4f + 0.5f;
			g = g * 0.4f + 0.5f;
			b = b * 0.4f + 0.5f;

			auto ur = unsigned(r * 255.0f);
			auto ug = unsigned(g * 255.0f);
			auto ub = unsigned(b * 255.0f);

			uint32_t color = (ur << 24) | (ug << 16) | (ub << 8);
			base[y * SCANOUT_WIDTH + x] = color;
		}
	}
}

static void event_func(const MTY_Event *evt, void *opaque)
{
	struct context *ctx = (struct context *) opaque;

	if (evt->type == MTY_EVENT_CLOSE)
		ctx->quit = true;
}

static bool app_func(void *opaque)
{
	struct context *ctx = (struct context *) opaque;

	ctx->processor.begin_frame_context();

	update_vram(ctx->rdram_ptr, ctx->frame_count++);
	render_frame(ctx, *ctx->device, *ctx->processor);

	ctx->processor.end_frame_context();

	MTY_WindowPresent(ctx->app, ctx->window);

	return !ctx->quit;
}

int32_t main(int32_t argc, char **argv)
{
	struct context ctx = {0};
	ctx.app = MTY_AppCreate((MTY_AppFlag) 0, app_func, event_func, &ctx);
	ctx.window = MTY_WindowCreate(ctx.app, "rdp-parallel", NULL, 0);

	MTY_WindowSetGFX(ctx.app, 0, MTY_GetDefaultGFX(), true);

	if (!Vulkan::Context::init_loader(nullptr)) {
		LOGE("Failed to initialize Vulkan loader.\n");
		return EXIT_FAILURE;
	}

	static const VkApplicationInfo info = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		"parallel-rdp-test", 0,
		"parallel-rdp", 0,
		VK_API_VERSION_1_1
	};

	std::vector<const char *> instance_ext = {};
	std::vector<const char *> device_ext = {};
	Vulkan::Context context;
	context.set_application_info(&info);
	context.set_num_thread_indices(1);

	if (!context.init_instance(instance_ext.data(), instance_ext.size(),
		Vulkan::ContextCreationFlagBits::CONTEXT_CREATION_ENABLE_ADVANCED_WSI_BIT))
	{
		LOGE("Failed to create Vulkan instance.\n");
		return EXIT_FAILURE;
	}

	if (!context.init_device(VK_NULL_HANDLE, VK_NULL_HANDLE, device_ext.data(), device_ext.size(),
		Vulkan::ContextCreationFlagBits::CONTEXT_CREATION_ENABLE_ADVANCED_WSI_BIT))
	{
		LOGE("Failed to create Vulkan device.\n");
		return EXIT_FAILURE;
	}

	Vulkan::Device device;
	device.set_context(context);
	ctx.device = &device;

	uint32_t RDRAM_SIZE = 4 * 1024 * 1024;
	ctx.rdram_ptr = Util::memalign_calloc(64 * 1024, RDRAM_SIZE);

	ctx.processor = std::make_unique<RDP::CommandProcessor>(
		device, ctx.rdram_ptr, 0, RDRAM_SIZE, RDRAM_SIZE / 8, RDP::CommandProcessorFlags{});

	if (!ctx.processor->device_is_supported()) {
		LOGE("Vulkan device does not support required features for parallel-rdp.\n");
		return EXIT_FAILURE;
	}

	setup_default_vi_registers(*ctx.processor);

	MTY_AppRun(ctx.app);

	ctx.processor.reset();
	Util::memalign_free(ctx.rdram_ptr);

	MTY_AppDestroy(&ctx.app);

	return 0;
}
