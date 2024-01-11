#include "Shared/Video/VideoRenderer.h"
#include "Shared/Video/DebugHud.h"
#include "Shared/Video/SystemHud.h"
#include "Shared/InputHud.h"
#include "Utilities/Video/IVideoRecorder.h"

VideoRenderer::VideoRenderer(Emulator* emu)
{
}

VideoRenderer::~VideoRenderer()
{
}

FrameInfo VideoRenderer::GetRendererSize()
{
	return {};
}

void VideoRenderer::SetRendererSize(uint32_t width, uint32_t height)
{
}

void VideoRenderer::StartThread()
{
}

void VideoRenderer::StopThread()
{
}

void VideoRenderer::RenderThread()
{
}

FrameInfo VideoRenderer::GetEmuHudSize(FrameInfo baseFrameSize)
{
	return {};
}

bool VideoRenderer::DrawScriptHud(RenderedFrame& frame)
{
	return false;
}

std::pair<FrameInfo, OverscanDimensions> VideoRenderer::GetScriptHudSize()
{
	return {{}, {}};
}

void VideoRenderer::UpdateFrame(RenderedFrame& frame)
{
}

void VideoRenderer::ClearFrame()
{
}

void VideoRenderer::RegisterRenderingDevice(IRenderingDevice *renderer)
{
}

void VideoRenderer::UnregisterRenderingDevice(IRenderingDevice *renderer)
{
}

void VideoRenderer::ProcessAviRecording(RenderedFrame& frame)
{
}

void VideoRenderer::StartRecording(string filename, RecordAviOptions options)
{
}

void VideoRenderer::AddRecordingSound(int16_t* soundBuffer, uint32_t sampleCount, uint32_t sampleRate)
{
}

void VideoRenderer::StopRecording()
{
}

bool VideoRenderer::IsRecording()
{
	return false;
}
