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

static CoreAudioFunc CORE_AUDIO;
static void *CORE_AUDIO_OPAQUE;
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

	ConsoleType t = _emu->GetConsole()->GetConsoleType();

	double multi = 0;

	if (t == ConsoleType::Nes)
		multi = 1.6;

	if (t == ConsoleType::Gameboy)
		multi = 1.3;

	if (multi > 1) {
		// Audio volume seems to be lower compared to previous versions of Mesen / other emulators?
		for (uint32_t x = 0; x < sampleCount * 2; x++)
			samples[x] = lrint(samples[x] * multi);
	}

	for (IAudioProvider *provider : _audioProviders)
		provider->MixAudio(samples, sampleCount, sourceRate);

	CORE_AUDIO(samples, sampleCount, sourceRate, CORE_AUDIO_OPAQUE);
}

double SoundMixer::GetRateAdjustment()
{
	return 1.0;
}

bool SoundMixer::IsRecording()
{
	return false;
}

void sound_mixer_set_func(CoreAudioFunc func, void *opaque)
{
	CORE_AUDIO = func;
	CORE_AUDIO_OPAQUE = opaque;
}
