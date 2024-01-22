#include "util/audio_stream.h"

#define AUDIO_STREAM_MAX (16 * 1024)

static s16 AUDIO_STREAM_BUF[AUDIO_STREAM_MAX];

class soundtouch::SoundTouch {
};

AudioStream::AudioStream(u32 sample_rate, u32 channels, u32 buffer_ms, AudioStretchMode stretch) :
	m_sample_rate(sample_rate), m_channels(channels), m_buffer_ms(buffer_ms), m_stretch_mode(stretch)
{
}

AudioStream::~AudioStream()
{
}

std::unique_ptr<AudioStream> AudioStream::CreateNullStream(u32 sample_rate, u32 channels, u32 buffer_ms)
{
	std::unique_ptr<AudioStream> stream(new AudioStream(sample_rate, channels, buffer_ms, AudioStretchMode::Off));

	return stream;
}

const char *AudioStream::GetStretchModeName(AudioStretchMode mode)
{
	return "None";
}

std::optional<AudioStretchMode> AudioStream::ParseStretchMode(const char* name)
{
	return AudioStretchMode::Off;
}

void AudioStream::BeginWrite(SampleType** buffer_ptr, u32* num_frames)
{
	*buffer_ptr = AUDIO_STREAM_BUF;
	*num_frames = AUDIO_STREAM_MAX / 2;
}

void AudioStream::EndWrite(u32 num_frames)
{
	// TODO Fire audio callback here
}

void AudioStream::SetNominalRate(float tempo)
{
}

void AudioStream::UpdateTargetTempo(float tempo)
{
}

void AudioStream::SetStretchMode(AudioStretchMode mode)
{
}


// Virtual

void AudioStream::SetPaused(bool paused)
{
}

void AudioStream::SetOutputVolume(u32 volume)
{
}
