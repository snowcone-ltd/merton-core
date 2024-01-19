#include "Shared/Video/VideoDecoder.h"
#include "Shared/Video/BaseVideoFilter.h"
#include "Shared/Video/ScaleFilter.h"
#include "Shared/Video/RotateFilter.h"
#include "Shared/Video/ScanlineFilter.h"

VideoDecoder::VideoDecoder(Emulator* emu)
{
}

VideoDecoder::~VideoDecoder()
{
}

void VideoDecoder::Init()
{
}

FrameInfo VideoDecoder::GetBaseFrameInfo(bool removeOverscan)
{
	return {};
}

FrameInfo VideoDecoder::GetFrameInfo()
{
	return {};
}

void VideoDecoder::UpdateVideoFilter()
{
}

void VideoDecoder::DecodeFrame(bool forRewind)
{
}

uint32_t VideoDecoder::GetFrameCount()
{
	return 0;
}

void VideoDecoder::WaitForAsyncFrameDecode()
{
}

void VideoDecoder::UpdateFrame(RenderedFrame frame, bool sync, bool forRewind)
{
}

void VideoDecoder::StartThread()
{
}

void VideoDecoder::StopThread()
{
}

bool VideoDecoder::IsRunning()
{
	return false;
}

void VideoDecoder::TakeScreenshot()
{
}

void VideoDecoder::TakeScreenshot(std::stringstream &stream)
{
}
