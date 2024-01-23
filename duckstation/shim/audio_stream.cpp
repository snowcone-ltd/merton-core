#include "util/audio_stream.h"

#include "../../core.h"

#define AUDIO_STREAM_MAX (16 * 1024)

static s16 AUDIO_STREAM_BUF[AUDIO_STREAM_MAX];
static CoreAudioFunc AUDIO_STREAM_FUNC;
static void *AUDIO_STREAM_OPAQUE;

class soundtouch::SoundTouch {
};

void core_log(const char *fmt, ...);

void audio_stream_set_func(CoreAudioFunc func, void *opaque)
{
	AUDIO_STREAM_FUNC = func;
	AUDIO_STREAM_OPAQUE = opaque;
}

AudioStream::AudioStream(u32 sample_rate, u32 channels, u32 buffer_ms, AudioStretchMode stretch) :
	m_sample_rate(sample_rate), m_channels(channels), m_buffer_ms(buffer_ms), m_stretch_mode(stretch)
{
	//m_buffer_size = m_buffer_ms * m_sample_rate / 1000;
	m_buffer_size = AUDIO_STREAM_MAX;
	m_target_buffer_size = m_buffer_size;
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
	if (AUDIO_STREAM_FUNC)
		AUDIO_STREAM_FUNC(AUDIO_STREAM_BUF, num_frames, m_sample_rate, AUDIO_STREAM_OPAQUE);
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
