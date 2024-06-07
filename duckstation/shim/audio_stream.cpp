// This prevents a dependency
#ifndef __ANDROID__
#define __ANDROID__
#endif

#include "util/audio_stream.h"

#undef __ANDROID__

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


AudioStream::AudioStream(u32 sample_rate, const AudioStreamParameters& parameters) :
	m_sample_rate(sample_rate)
{
	AUDIO_SAMPLE_RATE = sample_rate;

	m_buffer_size = parameters.buffer_ms * m_sample_rate / 1000;
}

AudioStream::~AudioStream()
{
}


std::unique_ptr<AudioStream> AudioStream::CreateNullStream(u32 sample_rate, u32 buffer_ms)
{
	AudioStreamParameters parameters = {};
	parameters.buffer_ms = buffer_ms;

	std::unique_ptr<AudioStream> stream(new AudioStream(sample_rate, parameters));

	return stream;
}

std::unique_ptr<AudioStream> AudioStream::CreateStream(AudioBackend backend, u32 sample_rate,
	const AudioStreamParameters& parameters, const char* driver_name, const char* device_name, Error* error)
{
	return AudioStream::CreateNullStream(sample_rate, parameters.buffer_ms);
}

u32 AudioStream::GetBufferedFramesRelaxed() const
{
	return 0;
}

const char *AudioStream::GetStretchModeName(AudioStretchMode mode)
{
	return "None";
}

const char* AudioStream::GetBackendName(AudioBackend backend)
{
	return "Null";
}

u32 AudioStream::GetMSForBufferSize(u32 sample_rate, u32 buffer_size)
{
	return 0;
}

const char* AudioStream::GetBackendDisplayName(AudioBackend backend)
{
	return "";
}

const char* AudioStream::GetExpansionModeName(AudioExpansionMode mode)
{
	return "Disabled";
}

std::optional<AudioBackend> AudioStream::ParseBackendName(const char* str)
{
	return AudioBackend::Null;
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


// AudioStreamParameters

void AudioStreamParameters::Load(SettingsInterface& si, const char* section)
{
	// XXX No code seems to load the defaults?
}

void AudioStreamParameters::Save(SettingsInterface& si, const char* section) const
{
}

void AudioStreamParameters::Clear(SettingsInterface& si, const char* section)
{
}

bool AudioStreamParameters::operator!=(const AudioStreamParameters& rhs) const
{
	return (std::memcmp(this, &rhs, sizeof(*this)) != 0);
}

bool AudioStreamParameters::operator==(const AudioStreamParameters& rhs) const
{
	return (std::memcmp(this, &rhs, sizeof(*this)) == 0);
}
