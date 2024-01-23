#include "util/audio_stream.h"

#include "common/align.h"
#include "common/assert.h"
#include "common/intrin.h"
#include "common/log.h"
#include "common/timer.h"

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

AudioStream::AudioStream(u32 sample_rate, u32 channels, u32 buffer_ms, AudioStretchMode stretch)
	: m_sample_rate(sample_rate), m_channels(channels), m_buffer_ms(buffer_ms), m_stretch_mode(stretch)
{
}

AudioStream::~AudioStream()
{
	DestroyBuffer();
}

std::unique_ptr<AudioStream> AudioStream::CreateNullStream(u32 sample_rate, u32 channels, u32 buffer_ms)
{
	std::unique_ptr<AudioStream> stream(new AudioStream(sample_rate, channels, buffer_ms, AudioStretchMode::Off));
	stream->BaseInitialize();

	return stream;
}

u32 AudioStream::GetAlignedBufferSize(u32 size)
{
	static_assert(Common::IsPow2(CHUNK_SIZE));
	return Common::AlignUpPow2(size, CHUNK_SIZE);
}

const char* AudioStream::GetStretchModeName(AudioStretchMode mode)
{
	return "None";
}

std::optional<AudioStretchMode> AudioStream::ParseStretchMode(const char* name)
{
	return std::nullopt;
}

u32 AudioStream::GetBufferedFramesRelaxed() const
{
	const u32 rpos = m_rpos.load(std::memory_order_relaxed);
	const u32 wpos = m_wpos.load(std::memory_order_relaxed);

	return (wpos + m_buffer_size - rpos) % m_buffer_size;
}

void AudioStream::ReadFrames(s16* bData, u32 nFrames)
{
	const u32 available_frames = GetBufferedFramesRelaxed();
	u32 frames_to_read = nFrames;
	u32 silence_frames = 0;

	if (m_filling) {
		u32 toFill = m_buffer_size / 32;
		toFill = GetAlignedBufferSize(toFill);

		if (available_frames < toFill) {
			silence_frames = nFrames;
			frames_to_read = 0;

		} else {
			m_filling = false;
		}
	}

	if (available_frames < frames_to_read) {
		silence_frames = frames_to_read - available_frames;
		frames_to_read = available_frames;
		m_filling = true;
	}

	if (frames_to_read > 0) {
		u32 rpos = m_rpos.load(std::memory_order_acquire);

		u32 end = m_buffer_size - rpos;
		if (end > frames_to_read)
			end = frames_to_read;

		if (end > 0) {
			std::memcpy(bData, &m_buffer[rpos], sizeof(s32) * end);
			rpos += end;
			rpos = (rpos == m_buffer_size) ? 0 : rpos;
		}

		const u32 start = frames_to_read - end;
		if (start > 0) {
			std::memcpy(&bData[end * 2], &m_buffer[0], sizeof(s32) * start);
			rpos = start;
		}

		m_rpos.store(rpos, std::memory_order_release);
	}

	if (silence_frames > 0)
		std::memset(bData + frames_to_read, 0, sizeof(s32) * silence_frames);
}

void AudioStream::InternalWriteFrames(s32* bData, u32 nSamples)
{
	const u32 free = m_buffer_size - GetBufferedFramesRelaxed();

	if (free <= nSamples)
		return;

	u32 wpos = m_wpos.load(std::memory_order_acquire);

	if ((m_buffer_size - wpos) <= nSamples) {
		const u32 end = m_buffer_size - wpos;
		const u32 start = nSamples - end;

		std::memcpy(&m_buffer[wpos], bData, end * sizeof(s32));
		if (start > 0)
			std::memcpy(&m_buffer[0], bData + end, start * sizeof(s32));

		wpos = start;

	} else {
		std::memcpy(&m_buffer[wpos], bData, nSamples * sizeof(s32));
		wpos += nSamples;
	}

	m_wpos.store(wpos, std::memory_order_release);

	/*
	size_t frames = GetBufferedFramesRelaxed();
	ReadFrames(AUDIO_STREAM_BUF, frames);

	if (AUDIO_STREAM_FUNC)
		AUDIO_STREAM_FUNC(AUDIO_STREAM_BUF, frames, m_sample_rate, AUDIO_STREAM_OPAQUE);
	*/
}

void AudioStream::BaseInitialize()
{
	AllocateBuffer();
}

void AudioStream::AllocateBuffer()
{
	const u32 multplier = 1;
	m_buffer_size = GetAlignedBufferSize(((m_buffer_ms * multplier) * m_sample_rate) / 1000);
	m_target_buffer_size = GetAlignedBufferSize((m_sample_rate * m_buffer_ms) / 1000u);
	m_buffer = std::unique_ptr<s32[]>(new s32[m_buffer_size]);
}

void AudioStream::DestroyBuffer()
{
	m_buffer.reset();
	m_buffer_size = 0;
	m_wpos.store(0, std::memory_order_release);
	m_rpos.store(0, std::memory_order_release);
}

void AudioStream::SetNominalRate(float tempo)
{
	m_nominal_rate = tempo;
}

void AudioStream::UpdateTargetTempo(float tempo)
{
}

void AudioStream::SetStretchMode(AudioStretchMode mode)
{
	if (m_stretch_mode == mode)
		return;

	bool paused = m_paused;
	if (!paused)
		SetPaused(true);

	DestroyBuffer();
	m_stretch_mode = mode;

	AllocateBuffer();

	if (!paused)
		SetPaused(false);
}

void AudioStream::SetPaused(bool paused)
{
	m_paused = paused;
}

void AudioStream::SetOutputVolume(u32 volume)
{
	m_volume = volume;
}

void AudioStream::BeginWrite(SampleType** buffer_ptr, u32* num_frames)
{
	*buffer_ptr = reinterpret_cast<s16*>(&m_staging_buffer[m_staging_buffer_pos]);
	*num_frames = CHUNK_SIZE - m_staging_buffer_pos;
}

void AudioStream::WriteFrames(const SampleType* frames, u32 num_frames)
{
}

void AudioStream::EndWrite(u32 num_frames)
{
	if (m_volume == 0)
		return;

	m_staging_buffer_pos += num_frames;
	DebugAssert(m_staging_buffer_pos <= CHUNK_SIZE);
	if (m_staging_buffer_pos < CHUNK_SIZE)
		return;

	m_staging_buffer_pos = 0;

	InternalWriteFrames(m_staging_buffer.data(), CHUNK_SIZE);
}
