#include "util/audio_stream.h"

#include "../../core.h"

#define AUDIO_STREAM_MAX (512 * 1024)

static int16_t AUDIO_STREAM_BUF[AUDIO_STREAM_MAX];
static uint32_t AUDIO_SAMPLE_RATE;
static size_t AUDIO_PTR;

static CoreAudioFunc CORE_AUDIO;
static void *CORE_AUDIO_OPAQUE;

class soundtouch::SoundTouch {
};

void core_log(const char *fmt, ...);

void audio_stream_set_func(CoreAudioFunc func, void *opaque)
{
	CORE_AUDIO = func;
	CORE_AUDIO_OPAQUE = opaque;
}

void audio_stream_finish(void)
{
	if (AUDIO_PTR > 0)
		CORE_AUDIO(AUDIO_STREAM_BUF, AUDIO_PTR / 2, AUDIO_SAMPLE_RATE, CORE_AUDIO_OPAQUE);

	AUDIO_PTR = 0;
}

AudioStream::AudioStream(u32 sample_rate, u32 channels, u32 buffer_ms, AudioStretchMode stretch) :
	m_sample_rate(sample_rate), m_channels(channels), m_buffer_ms(buffer_ms), m_stretch_mode(stretch)
{
	AUDIO_SAMPLE_RATE = sample_rate;

	m_buffer_size = m_buffer_ms * m_sample_rate / 1000;
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
	*buffer_ptr = AUDIO_STREAM_BUF + AUDIO_PTR;
	*num_frames = CHUNK_SIZE;
}

void AudioStream::EndWrite(u32 num_frames)
{
	AUDIO_PTR += num_frames * 2;

	if (AUDIO_PTR >= CHUNK_SIZE * 5)
		audio_stream_finish();
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
