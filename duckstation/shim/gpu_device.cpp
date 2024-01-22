#include "util/gpu_device.h"

std::unique_ptr<GPUDevice> g_gpu_device;
size_t GPUDevice::s_total_vram_usage;

void core_log(const char *fmt, ...);


// NullShader

class NullShader final : public GPUShader {
public:
	NullShader(GPUShaderStage stage);
	void SetDebugName(const std::string_view& name);
};

GPUShader::GPUShader(GPUShaderStage stage) : m_stage(stage)
{
}

GPUShader::~GPUShader()
{
}

NullShader::NullShader(GPUShaderStage stage) : GPUShader(stage)
{
}

void NullShader::SetDebugName(const std::string_view& name)
{
}


// NullPipeline

class NullPipeline final : public GPUPipeline {
public:
	NullPipeline();
	void SetDebugName(const std::string_view& name);
};

GPUPipeline::GPUPipeline()
{
}

GPUPipeline::~GPUPipeline()
{
}

NullPipeline::NullPipeline() : GPUPipeline()
{
}

void NullPipeline::SetDebugName(const std::string_view& name)
{
}


// NullDevice

class NullDevice final : public GPUDevice {
public:
	NullDevice();
	RenderAPI NullDevice::GetRenderAPI() const;
	bool HasSurface() const;
	void DestroySurface();
	bool UpdateWindow();
	AdapterAndModeList GetAdapterAndModeList();
	void UnmapUniformBuffer(u32 size);
	void SetRenderTargets(GPUTexture* const* rts, u32 num_rts, GPUTexture* ds);
	void SetPipeline(GPUPipeline* pipeline);
	void SetTextureSampler(u32 slot, GPUTexture* texture, GPUSampler* sampler);
	void SetTextureBuffer(u32 slot, GPUTextureBuffer* buffer);
	void SetViewport(s32 x, s32 y, s32 width, s32 height);
	void SetScissor(s32 x, s32 y, s32 width, s32 height);
	void Draw(u32 vertex_count, u32 base_vertex);
	void DrawIndexed(u32 index_count, u32 base_index, u32 base_vertex);
	void EndPresent();
	void SetVSync(bool enabled);
	void PushDebugGroup(const char* name);
	void PopDebugGroup();
	void InsertDebugMessage(const char* msg);
	void MapVertexBuffer(u32 vertex_size, u32 vertex_count, void** map_ptr, u32* map_space, u32* map_base_vertex);
	void UnmapVertexBuffer(u32 vertex_size, u32 vertex_count);
	void MapIndexBuffer(u32 index_count, DrawIndex** map_ptr, u32* map_space, u32* map_base_index);
	void UnmapIndexBuffer(u32 used_size);
	void PushUniformBuffer(const void* data, u32 data_size);
	void ResizeWindow(s32 new_window_width, s32 new_window_height, float new_window_scale);
	void CopyTextureRegion(GPUTexture* dst, u32 dst_x, u32 dst_y, u32 dst_layer, u32 dst_level,
		GPUTexture* src, u32 src_x, u32 src_y, u32 src_layer, u32 src_level, u32 width, u32 height);
	void ResolveTextureRegion(GPUTexture* dst, u32 dst_x, u32 dst_y, u32 dst_layer, u32 dst_level,
		GPUTexture* src, u32 src_x, u32 src_y, u32 width, u32 height);
	std::string GetDriverInfo() const;
	bool BeginPresent(bool skip_present);
	bool SupportsTextureFormat(GPUTexture::Format format) const;
	bool DownloadTexture(GPUTexture* texture, u32 x, u32 y, u32 width, u32 height,
		void* out_data, u32 out_data_stride);
	std::unique_ptr<GPUTexture> CreateTexture(u32 width, u32 height, u32 layers, u32 levels, u32 samples,
		GPUTexture::Type type, GPUTexture::Format format, const void* data = nullptr, u32 data_stride = 0);
	std::unique_ptr<GPUSampler> CreateSampler(const GPUSampler::Config& config);
	std::unique_ptr<GPUTextureBuffer> CreateTextureBuffer(GPUTextureBuffer::Format format, u32 size_in_elements);
	std::unique_ptr<GPUPipeline> CreatePipeline(const GPUPipeline::GraphicsConfig& config);
	void* MapUniformBuffer(u32 size);

protected:
	bool CreateDevice(const std::string_view& adapter, bool threaded_presentation,
		std::optional<bool> exclusive_fullscreen_control, FeatureMask disabled_features);
	void DestroyDevice();
	std::unique_ptr<GPUShader> CreateShaderFromBinary(GPUShaderStage stage, std::span<const u8> data);
	std::unique_ptr<GPUShader> CreateShaderFromSource(GPUShaderStage stage, const std::string_view& source,
		const char* entry_point, DynamicHeapArray<u8>* out_binary);
};

bool GPUDevice::Create(const std::string_view& adapter, const std::string_view& shader_cache_path,
	u32 shader_cache_version, bool debug_device, bool vsync, bool threaded_presentation,
	std::optional<bool> exclusive_fullscreen_control, FeatureMask disabled_features)
{
	m_vsync_enabled = vsync;
	m_debug_device = debug_device;

	m_max_texture_size = INT32_MAX;
	m_max_multisamples = INT32_MAX;

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

std::unique_ptr<GPUShader> GPUDevice::CreateShader(GPUShaderStage stage, const std::string_view& source,
	const char* entry_point)
{
	std::unique_ptr<NullShader> shader(new NullShader(stage));

	return shader;
}

std::unique_ptr<GPUTexture> GPUDevice::FetchTexture(u32 width, u32 height, u32 layers, u32 levels, u32 samples,
	GPUTexture::Type type, GPUTexture::Format format, const void* data, u32 data_stride)
{
	core_log("FetchTexture\n");

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
	std::unique_ptr<GPUDevice> device(new NullDevice());

	return device;
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


// Virutal

NullDevice::NullDevice()
{
}

GPUDevice::~GPUDevice()
{
}

bool GPUDevice::SupportsExclusiveFullscreen() const
{
	return false;
}

void GPUDevice::ClearRenderTarget(GPUTexture* t, u32 c)
{
	core_log("ClearRenderTarget\n");

	t->SetClearColor(c);
}

void GPUDevice::ClearDepth(GPUTexture* t, float d)
{
	core_log("ClearDepth\n");

	t->SetClearDepth(d);
}

void GPUDevice::InvalidateRenderTarget(GPUTexture* t)
{
	core_log("InvalidateRanderTarget\n");

	t->SetState(GPUTexture::State::Invalidated);
}

bool GPUDevice::GetHostRefreshRate(float* refresh_rate)
{
	*refresh_rate = 60;

	return true;
}

bool GPUDevice::SetGPUTimingEnabled(bool enabled)
{
	core_log("SetGPUTimingEnabled\n");

	return false;
}

float GPUDevice::GetAndResetAccumulatedGPUTime()
{
	core_log("GetAndResetAccumulatedGPUTime\n");

	return 0.0f;
}


// Virtual protected

bool GPUDevice::ReadPipelineCache(const std::string& filename)
{
	core_log("ReadPipelineCache\n");

	return false;
}

bool GPUDevice::GetPipelineCacheData(DynamicHeapArray<u8>* data)
{
	return false;
}


// Pure virtual

RenderAPI NullDevice::GetRenderAPI() const
{
	return RenderAPI::OpenGL;
}

bool NullDevice::HasSurface() const
{
	return true;
}

void NullDevice::DestroySurface()
{
}

bool NullDevice::UpdateWindow()
{
	return true;
}

GPUDevice::AdapterAndModeList NullDevice::GetAdapterAndModeList()
{
	core_log("GetAdapterAndModeList\n");

	return {};
}

void NullDevice::UnmapUniformBuffer(u32 size)
{
}

void NullDevice::SetRenderTargets(GPUTexture* const* rts, u32 num_rts, GPUTexture* ds)
{
}

void NullDevice::SetPipeline(GPUPipeline* pipeline)
{
}

void NullDevice::SetTextureSampler(u32 slot, GPUTexture* texture, GPUSampler* sampler)
{
}

void NullDevice::SetTextureBuffer(u32 slot, GPUTextureBuffer* buffer)
{
}

void NullDevice::SetViewport(s32 x, s32 y, s32 width, s32 height)
{
}

void NullDevice::SetScissor(s32 x, s32 y, s32 width, s32 height)
{
}

void NullDevice::Draw(u32 vertex_count, u32 base_vertex)
{
}

void NullDevice::DrawIndexed(u32 index_count, u32 base_index, u32 base_vertex)
{
}

void NullDevice::EndPresent()
{
}

void NullDevice::SetVSync(bool enabled)
{
}

void NullDevice::PushDebugGroup(const char* name)
{
}

void NullDevice::PopDebugGroup()
{
}

void NullDevice::InsertDebugMessage(const char* msg)
{
}

void NullDevice::UnmapVertexBuffer(u32 vertex_size, u32 vertex_count)
{
}

void NullDevice::UnmapIndexBuffer(u32 used_size)
{
}

void NullDevice::PushUniformBuffer(const void* data, u32 data_size)
{
}

void NullDevice::ResizeWindow(s32 new_window_width, s32 new_window_height, float new_window_scale)
{
}

void NullDevice::CopyTextureRegion(GPUTexture* dst, u32 dst_x, u32 dst_y, u32 dst_layer, u32 dst_level,
	GPUTexture* src, u32 src_x, u32 src_y, u32 src_layer, u32 src_level, u32 width, u32 height)
{
}

void NullDevice::ResolveTextureRegion(GPUTexture* dst, u32 dst_x, u32 dst_y, u32 dst_layer, u32 dst_level,
	GPUTexture* src, u32 src_x, u32 src_y, u32 width, u32 height)
{
}

std::string NullDevice::GetDriverInfo() const
{
	return "Null";
}

bool NullDevice::BeginPresent(bool skip_present)
{
	return true;
}

bool NullDevice::SupportsTextureFormat(GPUTexture::Format format) const
{
	return true;
}

bool NullDevice::DownloadTexture(GPUTexture* texture, u32 x, u32 y, u32 width, u32 height,
	void* out_data, u32 out_data_stride)
{
	core_log("DownloadTexture\n");

	return true;
}

void NullDevice::MapIndexBuffer(u32 index_count, DrawIndex** map_ptr, u32* map_space, u32* map_base_index)
{
	core_log("MapIndexBuffer\n");
}

void NullDevice::MapVertexBuffer(u32 vertex_size, u32 vertex_count, void** map_ptr, u32* map_space, u32* map_base_vertex)
{
	core_log("MapVertexBuffer\n");
}

void* NullDevice::MapUniformBuffer(u32 size)
{
	core_log("MapUniformBuffer\n");

	return NULL;
}

std::unique_ptr<GPUTexture> NullDevice::CreateTexture(u32 width, u32 height, u32 layers, u32 levels, u32 samples,
	GPUTexture::Type type, GPUTexture::Format format, const void* data, u32 data_stride)
{
	core_log("CreateTexture\n");

	return {};
}

std::unique_ptr<GPUSampler> NullDevice::CreateSampler(const GPUSampler::Config& config)
{
	core_log("CreateSampler\n");

	return {};
}

std::unique_ptr<GPUTextureBuffer> NullDevice::CreateTextureBuffer(GPUTextureBuffer::Format format, u32 size_in_elements)
{
	core_log("CreateTextureBuffer\n");

	return {};
}

std::unique_ptr<GPUPipeline> NullDevice::CreatePipeline(const GPUPipeline::GraphicsConfig& config)
{
	std::unique_ptr<GPUPipeline> pipeline(new NullPipeline());

	return pipeline;
}


// Pure virtual protected

bool NullDevice::CreateDevice(const std::string_view& adapter, bool threaded_presentation,
	std::optional<bool> exclusive_fullscreen_control, FeatureMask disabled_features)
{
	return true;
}

void NullDevice::DestroyDevice()
{
}

std::unique_ptr<GPUShader> NullDevice::CreateShaderFromBinary(GPUShaderStage stage, std::span<const u8> data)
{
	return {};
}

std::unique_ptr<GPUShader> NullDevice::CreateShaderFromSource(GPUShaderStage stage, const std::string_view& source,
	const char* entry_point, DynamicHeapArray<u8>* out_binary)
{
	return {};
}
