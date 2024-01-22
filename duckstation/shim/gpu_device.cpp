#include "util/gpu_device.h"

std::unique_ptr<GPUDevice> g_gpu_device;
size_t GPUDevice::s_total_vram_usage;


// NullDevice

class NullDevice : GPUDevice {
	NullDevice();
};

NullDevice::NullDevice()
{
}


// GPUDevice

GPUDevice::~GPUDevice()
{
}

bool GPUDevice::Create(const std::string_view& adapter, const std::string_view& shader_cache_path,
	u32 shader_cache_version, bool debug_device, bool vsync, bool threaded_presentation,
	std::optional<bool> exclusive_fullscreen_control, FeatureMask disabled_features)
{
	m_vsync_enabled = vsync;
	m_debug_device = debug_device;

	m_max_texture_size = UINT32_MAX;
	m_max_multisamples = UINT32_MAX;

	m_features = {};
	// m_features.dual_source_blend : 1;
	// m_features.framebuffer_fetch : 1;
	// m_features.per_sample_shading : 1;
	// m_features.noperspective_interpolation : 1;
	// m_features.texture_copy_to_self : 1;
	// m_features.supports_texture_buffers : 1;
	// m_features.texture_buffers_emulated_with_ssbo : 1;
	// m_features.geometry_shaders : 1;
	// m_features.partial_msaa_resolve : 1;
	// m_features.gpu_timing : 1;
	// m_features.shader_cache : 1;
	// m_features.pipeline_cache : 1;
	// m_features.prefer_unused_textures : 1;

	m_window_info = {};
	m_window_info.surface_width = 1920;
	m_window_info.surface_height = 1080;
	m_window_info.surface_refresh_rate = 60.0f;
	m_window_info.surface_scale = 1.0f;
	m_window_info.surface_format = GPUTexture::Format::Unknown;

	m_nearest_sampler = {};
	m_linear_sampler = {};

	return true;
}

RenderAPI GPUDevice::GetPreferredAPI()
{
	// return RenderAPI::None;
	return RenderAPI::OpenGL;
}

bool GPUDevice::ShouldSkipDisplayingFrame()
{
	return false;
}

bool GPUDevice::GetHostRefreshRate(float* refresh_rate)
{
	*refresh_rate = 60;

	return true;
}

std::unique_ptr<GPUShader> GPUDevice::CreateShader(GPUShaderStage stage, const std::string_view& source,
	const char* entry_point)
{
	return {};
}

std::unique_ptr<GPUTexture> GPUDevice::FetchTexture(u32 width, u32 height, u32 layers, u32 levels, u32 samples,
	GPUTexture::Type type, GPUTexture::Format format, const void* data, u32 data_stride)
{
	return {};
}

void GPUDevice::RecycleTexture(std::unique_ptr<GPUTexture> texture)
{
}

void GPUDevice::SetViewportAndScissor(s32 x, s32 y, s32 width, s32 height)
{
}

void GPUDevice::SetRenderTarget(GPUTexture* rt, GPUTexture* ds)
{
}

void GPUDevice::UploadUniformBuffer(const void* data, u32 data_size)
{
}


// Unmodified

GPUShaderCache::GPUShaderCache()
{
}

GPUShaderCache::~GPUShaderCache()
{
}

void GPUDevice::SetDisplayMaxFPS(float max_fps)
{
	m_display_frame_interval = (max_fps > 0.0f) ? (1.0f / max_fps) : 0.0f;
}

void GPUPipeline::GraphicsConfig::SetTargetFormats(GPUTexture::Format color_format,
	GPUTexture::Format depth_format_)
{
	color_formats[0] = color_format;

	for (size_t i = 1; i < std::size(color_formats); i++)
		color_formats[i] = GPUTexture::Format::Unknown;

	depth_format = depth_format_;
}

std::array<float, 4> GPUDevice::RGBA8ToFloat(u32 rgba)
{
	return std::array<float, 4>{static_cast<float>(rgba & UINT32_C(0xFF)) * (1.0f / 255.0f),
		static_cast<float>((rgba >> 8) & UINT32_C(0xFF)) * (1.0f / 255.0f),
		static_cast<float>((rgba >> 16) & UINT32_C(0xFF)) * (1.0f / 255.0f),
		static_cast<float>(rgba >> 24) * (1.0f / 255.0f)};
}

GPUPipeline::BlendState GPUPipeline::BlendState::GetAlphaBlendingState()
{
	BlendState ret = {};
	ret.enable = true;
	ret.src_blend = BlendFunc::SrcAlpha;
	ret.dst_blend = BlendFunc::InvSrcAlpha;
	ret.blend_op = BlendOp::Add;
	ret.src_alpha_blend = BlendFunc::One;
	ret.dst_alpha_blend = BlendFunc::Zero;
	ret.alpha_blend_op = BlendOp::Add;
	ret.write_mask = 0xf;

	return ret;
}

GPUPipeline::DepthState GPUPipeline::DepthState::GetAlwaysWriteState()
{
	DepthState ret = {};
	ret.depth_test = DepthFunc::Always;
	ret.depth_write = true;

	return ret;
}

GPUSampler::Config GPUSampler::GetLinearConfig()
{
	Config config = {};
	config.address_u = GPUSampler::AddressMode::ClampToEdge;
	config.address_v = GPUSampler::AddressMode::ClampToEdge;
	config.address_w = GPUSampler::AddressMode::ClampToEdge;
	config.min_filter = GPUSampler::Filter::Linear;
	config.mag_filter = GPUSampler::Filter::Linear;

	return config;
}

Common::Rectangle<s32> GPUDevice::FlipToLowerLeft(const Common::Rectangle<s32>& rc, s32 target_height)
{
	const s32 height = rc.GetHeight();
	const s32 flipped_y = target_height - rc.top - height;

	return Common::Rectangle<s32>(rc.left, flipped_y, rc.right, flipped_y + height);
}

bool GPUDevice::UsesLowerLeftOrigin() const
{
	const RenderAPI api = GetRenderAPI();

	return api == RenderAPI::OpenGL || api == RenderAPI::OpenGLES;
}

std::unique_ptr<GPUTexture, GPUDevice::PooledTextureDeleter> GPUDevice::FetchAutoRecycleTexture(u32 width,
	u32 height, u32 layers, u32 levels, u32 samples, GPUTexture::Type type, GPUTexture::Format format,
	const void* data, u32 data_stride, bool dynamic)
{
	std::unique_ptr<GPUTexture> ret =
		FetchTexture(width, height, layers, levels, samples, type, format, data, data_stride);

	return std::unique_ptr<GPUTexture, PooledTextureDeleter>(ret.release());
}

GPUPipeline::BlendState GPUPipeline::BlendState::GetNoBlendingState()
{
	BlendState ret = {};
	ret.write_mask = 0xf;

	return ret;
}

GPUPipeline::DepthState GPUPipeline::DepthState::GetNoTestsState()
{
	DepthState ret = {};
	ret.depth_test = DepthFunc::Always;

	return ret;
}

GPUPipeline::RasterizationState GPUPipeline::RasterizationState::GetNoCullState()
{
	RasterizationState ret = {};
	ret.cull_mode = CullMode::None;

	return ret;
}

const char* GPUDevice::RenderAPIToString(RenderAPI api)
{
	switch (api) {
		#define CASE(x) case RenderAPI::x: return #x
		CASE(None);
		CASE(D3D11);
		CASE(D3D12);
		CASE(Metal);
		CASE(Vulkan);
		CASE(OpenGL);
		CASE(OpenGLES);
		#undef CASE

		default:
			return "Unknown";
	}
}

std::unique_ptr<GPUDevice> GPUDevice::CreateDeviceForAPI(RenderAPI api)
{
	//std::unique_ptr<GPUDevice> device(new NullDevice());

	//return device;

	// FIXME This needs to be instantiated
	return {};
}

void GPUDevice::Destroy()
{
}

GPUSampler::Config GPUSampler::GetNearestConfig()
{
	Config config = {};
	config.address_u = GPUSampler::AddressMode::ClampToEdge;
	config.address_v = GPUSampler::AddressMode::ClampToEdge;
	config.address_w = GPUSampler::AddressMode::ClampToEdge;
	config.min_filter = GPUSampler::Filter::Nearest;
	config.mag_filter = GPUSampler::Filter::Nearest;

	return config;
}

bool GPUDevice::IsSameRenderAPI(RenderAPI lhs, RenderAPI rhs)
{
	return (lhs == rhs || ((lhs == RenderAPI::OpenGL || lhs == RenderAPI::OpenGLES) &&
		(rhs == RenderAPI::OpenGL || rhs == RenderAPI::OpenGLES)));
}

void GPUDevice::RenderImGui()
{
}

bool GPUDevice::ReadPipelineCache(const std::string& filename)
{
	return false;
}

bool GPUDevice::GetPipelineCacheData(DynamicHeapArray<u8>* data)
{
	return false;
}

bool GPUDevice::SupportsExclusiveFullscreen() const
{
	return false;
}

void GPUDevice::ClearRenderTarget(GPUTexture* t, u32 c)
{
	t->SetClearColor(c);
}

void GPUDevice::ClearDepth(GPUTexture* t, float d)
{
	t->SetClearDepth(d);
}

void GPUDevice::InvalidateRenderTarget(GPUTexture* t)
{
	t->SetState(GPUTexture::State::Invalidated);
}

bool GPUDevice::SetGPUTimingEnabled(bool enabled)
{
	return false;
}

float GPUDevice::GetAndResetAccumulatedGPUTime()
{
	return 0.0f;
}
