#include "util/gpu_device.h"

#include "../../core.h"

std::unique_ptr<GPUDevice> g_gpu_device;
size_t GPUDevice::s_total_vram_usage;
GPUDevice::Statistics GPUDevice::s_stats;

static CoreVideoFunc CORE_VIDEO;
static void *CORE_VIDEO_OPAQUE;
static bool GPU_DEVICE_GOT_FRAME;

void core_log(const char *fmt, ...);

void gpu_device_set_func(CoreVideoFunc func, void *opaque)
{
	CORE_VIDEO = func;
	CORE_VIDEO_OPAQUE = opaque;
}

void gpu_device_finish(void)
{
	if (!GPU_DEVICE_GOT_FRAME)
		CORE_VIDEO(NULL, CORE_COLOR_FORMAT_UNKNOWN, 0, 0, 0, CORE_VIDEO_OPAQUE);

	GPU_DEVICE_GOT_FRAME = false;
}


// NullTexture

class NullTexture final : public GPUTexture {
public:
	NullTexture(u16 width, u16 height, u8 layers, u8 levels, u8 samples, Type type, Format format);
	void SetDebugName(const std::string_view& name);
	bool Update(u32 x, u32 y, u32 width, u32 height, const void* data, u32 pitch, u32 layer = 0, u32 level = 0);
	bool Map(void** map, u32* map_stride, u32 x, u32 y, u32 width, u32 height, u32 layer = 0, u32 level = 0);
	void Unmap();
};

NullTexture::NullTexture(u16 width, u16 height, u8 layers, u8 levels, u8 samples, Type type, Format format) :
	GPUTexture(static_cast<u16>(width), static_cast<u16>(height), static_cast<u8>(layers), static_cast<u8>(levels),
	static_cast<u8>(samples), type, format)
{
}

void NullTexture::SetDebugName(const std::string_view& name)
{
}

bool NullTexture::Update(u32 x, u32 y, u32 width, u32 height, const void* data, u32 pitch, u32 layer, u32 level)
{
	CoreColorFormat f = CORE_COLOR_FORMAT_UNKNOWN;

	switch (m_format) {
		case GPUTexture::Format::RGB565:
			f = CORE_COLOR_FORMAT_B5G6R5;
			break;
		case GPUTexture::Format::RGBA8:
			f = CORE_COLOR_FORMAT_RGBA;
			break;
		default:
			core_log("Unknown texture format %d\n", m_format);
			return false;
	}

	CORE_VIDEO(data, f, width, height, pitch, CORE_VIDEO_OPAQUE);

	GPU_DEVICE_GOT_FRAME = true;

	return true;
}

bool NullTexture::Map(void** map, u32* map_stride, u32 x, u32 y, u32 width, u32 height, u32 layer, u32 level)
{
	return false;
}

void NullTexture::Unmap()
{
}


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
	RenderAPI GetRenderAPI() const;
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
		std::optional<bool> exclusive_fullscreen_control, FeatureMask disabled_features, Error* error);
	void DestroyDevice();
	std::unique_ptr<GPUShader> CreateShaderFromBinary(GPUShaderStage stage, std::span<const u8> data);
	std::unique_ptr<GPUShader> CreateShaderFromSource(GPUShaderStage stage, const std::string_view& source,
		const char* entry_point, DynamicHeapArray<u8>* out_binary);
};

GPUDevice::GPUDevice()
{
}

GPUDevice::~GPUDevice()
{
}

NullDevice::NullDevice() : GPUDevice()
{
}

bool GPUDevice::Create(const std::string_view& adapter, const std::string_view& shader_cache_path,
	u32 shader_cache_version, bool debug_device, bool vsync, bool threaded_presentation,
	std::optional<bool> exclusive_fullscreen_control, FeatureMask disabled_features, Error* error)
{
	return true;
}

RenderAPI GPUDevice::GetPreferredAPI()
{
	return RenderAPI::None;
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
	std::unique_ptr<NullTexture> tex(new NullTexture(width, height, layers, levels, samples, type, format));

	return tex;
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
}

void GPUPipeline::GraphicsConfig::SetTargetFormats(GPUTexture::Format color_format,
	GPUTexture::Format depth_format_)
{
}

std::array<float, 4> GPUDevice::RGBA8ToFloat(u32 rgba)
{
	return std::array<float, 4>{0, 0, 0, 0};
}

GPUPipeline::BlendState GPUPipeline::BlendState::GetAlphaBlendingState()
{
	return {};
}

GPUPipeline::DepthState GPUPipeline::DepthState::GetAlwaysWriteState()
{
	return {};
}

GPUSampler::Config GPUSampler::GetLinearConfig()
{
	return {};
}

Common::Rectangle<s32> GPUDevice::FlipToLowerLeft(const Common::Rectangle<s32>& rc, s32 target_height)
{
	return {};
}

bool GPUDevice::UsesLowerLeftOrigin() const
{
	return false;
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
	return {};
}

GPUPipeline::DepthState GPUPipeline::DepthState::GetNoTestsState()
{
	return {};
}

GPUPipeline::RasterizationState GPUPipeline::RasterizationState::GetNoCullState()
{
	return {};
}

const char* GPUDevice::RenderAPIToString(RenderAPI api)
{
	return "Null";
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
	return {};
}

bool GPUDevice::IsSameRenderAPI(RenderAPI lhs, RenderAPI rhs)
{
	return lhs == rhs;
}

void GPUDevice::RenderImGui()
{
}

void GPUDevice::ResetStatistics()
{
}


// Virutal

bool GPUDevice::SupportsExclusiveFullscreen() const
{
	return false;
}

void GPUDevice::ClearRenderTarget(GPUTexture* t, u32 c)
{
}

void GPUDevice::ClearDepth(GPUTexture* t, float d)
{
}

void GPUDevice::InvalidateRenderTarget(GPUTexture* t)
{
}

bool GPUDevice::GetHostRefreshRate(float* refresh_rate)
{
	return false;
}

bool GPUDevice::SetGPUTimingEnabled(bool enabled)
{
	return false;
}

float GPUDevice::GetAndResetAccumulatedGPUTime()
{
	return 0.0f;
}


// Virtual protected

bool GPUDevice::ReadPipelineCache(const std::string& filename)
{
	return false;
}

bool GPUDevice::GetPipelineCacheData(DynamicHeapArray<u8>* data)
{
	return false;
}


// Pure virtual

RenderAPI NullDevice::GetRenderAPI() const
{
	return RenderAPI::None;
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
	return false;
}

void NullDevice::MapIndexBuffer(u32 index_count, DrawIndex** map_ptr, u32* map_space, u32* map_base_index)
{
}

void NullDevice::MapVertexBuffer(u32 vertex_size, u32 vertex_count, void** map_ptr, u32* map_space, u32* map_base_vertex)
{
}

void* NullDevice::MapUniformBuffer(u32 size)
{
	return NULL;
}

std::unique_ptr<GPUTexture> NullDevice::CreateTexture(u32 width, u32 height, u32 layers, u32 levels, u32 samples,
	GPUTexture::Type type, GPUTexture::Format format, const void* data, u32 data_stride)
{
	return {};
}

std::unique_ptr<GPUSampler> NullDevice::CreateSampler(const GPUSampler::Config& config)
{
	return {};
}

std::unique_ptr<GPUTextureBuffer> NullDevice::CreateTextureBuffer(GPUTextureBuffer::Format format, u32 size_in_elements)
{
	return {};
}

std::unique_ptr<GPUPipeline> NullDevice::CreatePipeline(const GPUPipeline::GraphicsConfig& config)
{
	std::unique_ptr<GPUPipeline> pipeline(new NullPipeline());

	return pipeline;
}


// Pure virtual protected

bool NullDevice::CreateDevice(const std::string_view& adapter, bool threaded_presentation,
	std::optional<bool> exclusive_fullscreen_control, FeatureMask disabled_features, Error* error)
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
