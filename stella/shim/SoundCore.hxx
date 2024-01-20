#pragma once

#include <sstream>
#include <cassert>
#include <cmath>

#include "bspf.hxx"
#include "Logger.hxx"
#include "FrameBuffer.hxx"
#include "Settings.hxx"
#include "System.hxx"
#include "OSystem.hxx"
#include "Console.hxx"
#include "Sound.hxx"
#include "AudioQueue.hxx"
#include "EmulationTiming.hxx"
#include "AudioSettings.hxx"

class SoundCore : public Sound {
	public:
		SoundCore(OSystem& osystem, AudioSettings& audioSettings) : Sound(osystem),
			myAudioSettings{audioSettings}
		{
			Logger::debug("SoundCore::SoundCore started ...");
			Logger::debug("SoundCore::SoundCore initialized");
		}
		~SoundCore() override = default;

	public:
		void open(shared_ptr<AudioQueue> audioQueue, EmulationTiming* emulationTiming) override
		{
			myEmulationTiming = emulationTiming;

			Logger::debug("SoundCore::open started ...");

			audioQueue->ignoreOverflows(!myAudioSettings.enabled());

			myAudioQueue = audioQueue;
			myUnderrun = true;
			myCurrentFragment = nullptr;

			Logger::debug("SoundCore::open finished");

			myIsInitializedFlag = true;
		}

		void dequeue(Int16* stream, uInt32* samples)
		{
			uInt32 outIndex = 0;

			while (myAudioQueue->size()) {
				Int16* nextFragment = myAudioQueue->dequeue(myCurrentFragment);

				if (!nextFragment) {
					*samples = outIndex / 2;
					return;
				}

				myCurrentFragment = nextFragment;

				for (uInt32 i = 0; i < myAudioQueue->fragmentSize(); ++i) {
					Int16 sampleL, sampleR;

					if (myAudioQueue->isStereo()) {
						sampleL = static_cast<Int16>(myCurrentFragment[2*i + 0]);
						sampleR = static_cast<Int16>(myCurrentFragment[2*i + 1]);

					} else {
						sampleL = sampleR = static_cast<Int16>(myCurrentFragment[i]);
					}

					stream[outIndex++] = sampleL;
					stream[outIndex++] = sampleR;
				}
			}
			*samples = outIndex / 2;
		}

	protected:
		void setEnabled(bool enable) override {}
		void queryHardware(VariantList& devices) override {}
		void setVolume(uInt32 percent) override {}
		void adjustVolume(int direction = +1) override {}
		void mute(bool state) override {}
		void toggleMute() override {}
		bool pause(bool state) override {return !myIsInitializedFlag;}
		string about() const override {return "";}

	private:
		bool myIsInitializedFlag{false};

		shared_ptr<AudioQueue> myAudioQueue;

		EmulationTiming* myEmulationTiming{nullptr};

		Int16* myCurrentFragment{nullptr};
		bool myUnderrun{false};

		AudioSettings& myAudioSettings;

	private:
		SoundCore() = delete;
		SoundCore(const SoundCore&) = delete;
		SoundCore(SoundCore&&) = delete;
		SoundCore& operator=(const SoundCore&) = delete;
		SoundCore& operator=(SoundCore&&) = delete;
};
