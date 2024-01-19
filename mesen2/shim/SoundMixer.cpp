#include "Shared/Audio/SoundMixer.h"
#include "Shared/Audio/AudioPlayerHud.h"
#include "Shared/Audio/SoundResampler.h"
#include "Shared/Audio/WaveRecorder.h"
#include "Shared/Interfaces/IAudioProvider.h"
#include "Shared/Emulator.h"
#include "Utilities/Audio/Equalizer.h"
#include "Utilities/Audio/ReverbFilter.h"
#include "Utilities/Audio/CrossFeedFilter.h"

#include "../../core.h"

static CoreAudioFunc SOUND_MIXER_AUDIO_FUNC;
static void *SOUND_MIXER_AUDIO_OPAQUE;
static uint32_t SOUND_MIXER_SAMPLE_RATE;

SoundMixer::SoundMixer(Emulator* emu)
{
	_emu = emu;
}

SoundMixer::~SoundMixer()
{
}

void SoundMixer::RegisterAudioProvider(IAudioProvider* provider)
{
	_audioProviders.push_back(provider);
}

void SoundMixer::UnregisterAudioProvider(IAudioProvider* provider)
{
	vector<IAudioProvider*>& vec = _audioProviders;
	vec.erase(std::remove(vec.begin(), vec.end(), provider), vec.end());
}

AudioStatistics SoundMixer::GetStatistics()
{
	return AudioStatistics();
}

void SoundMixer::StopAudio(bool clearBuffer)
{
}

void SoundMixer::PlayAudioBuffer(int16_t* samples, uint32_t sampleCount, uint32_t sourceRate)
{
	SOUND_MIXER_SAMPLE_RATE = sourceRate;

	if (SOUND_MIXER_AUDIO_FUNC) {
		ConsoleType t = _emu->GetConsole()->GetConsoleType();

		if (t == ConsoleType::Nes) {
			// Audio volume seems to be lower compared to previous versions of Mesen / other emulators?
			for (uint32_t x = 0; x < sampleCount * 2; x++)
				samples[x] = lrint(samples[x] * 1.6);
		}

		for (IAudioProvider *provider : _audioProviders)
			provider->MixAudio(samples, sampleCount, sourceRate);

		SOUND_MIXER_AUDIO_FUNC(samples, sampleCount, sourceRate, SOUND_MIXER_AUDIO_OPAQUE);
	}
}

double SoundMixer::GetRateAdjustment()
{
	return 1.0;
}

bool SoundMixer::IsRecording()
{
	return false;
}

void sound_mixer_set_audio_func(CoreAudioFunc func, void *opaque)
{
	SOUND_MIXER_AUDIO_FUNC = func;
	SOUND_MIXER_AUDIO_OPAQUE = opaque;
}
