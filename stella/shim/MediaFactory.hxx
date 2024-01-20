#pragma once

#include "bspf.hxx"

#include "OSystem.hxx"
#include "Settings.hxx"
#include "SerialPort.hxx"

#if defined(BSPF_UNIX)
	#include "SerialPortUNIX.hxx"
	#if defined(RETRON77)
		#include "SettingsR77.hxx"
		#include "OSystemR77.hxx"
	#else
		#include "OSystemUNIX.hxx"
	#endif
#elif defined(BSPF_WINDOWS)
	#include "SerialPortWINDOWS.hxx"
	#include "OSystemWINDOWS.hxx"
#elif defined(BSPF_MACOS)
	#include "SerialPortMACOS.hxx"
	#include "OSystemMACOS.hxx"
	extern "C" {
		int stellaMain(int argc, char* argv[]);
	}
#else
	#error Unsupported platform!
#endif

#include "EventHandlerCore.hxx"
#include "FBBackendCore.hxx"
#include "SoundCore.hxx"

class AudioSettings;

class MediaFactory {
	public:
		static unique_ptr<OSystem> createOSystem()
		{
		#if defined(BSPF_UNIX)
			#if defined(RETRON77)
				return make_unique<OSystemR77>();
			#else
				return make_unique<OSystemUNIX>();
			#endif
		#elif defined(BSPF_WINDOWS)
			return make_unique<OSystemWINDOWS>();
		#elif defined(BSPF_MACOS)
			return make_unique<OSystemMACOS>();
		#else
			#error Unsupported platform for OSystem!
		#endif
		}

		static unique_ptr<Settings> createSettings()
		{
		#ifdef RETRON77
			return make_unique<SettingsR77>();
		#else
			return make_unique<Settings>();
		#endif
		}

		static unique_ptr<SerialPort> createSerialPort()
		{
		#if defined(BSPF_UNIX)
			return make_unique<SerialPortUNIX>();
		#elif defined(BSPF_WINDOWS)
			return make_unique<SerialPortWINDOWS>();
		#elif defined(BSPF_MACOS)
			return make_unique<SerialPortMACOS>();
		#else
			return make_unique<SerialPort>();
		#endif
		}

		static unique_ptr<FBBackend> createVideoBackend(OSystem& osystem)
		{
			return make_unique<FBBackendCore>(osystem);
		}

		static unique_ptr<Sound> createAudio(OSystem& osystem, AudioSettings& audioSettings)
		{
			return make_unique<SoundCore>(osystem, audioSettings);
		}

		static unique_ptr<EventHandler> createEventHandler(OSystem& osystem)
		{
			return make_unique<EventHandlerCore>(osystem);
		}

		static void cleanUp()
		{
		}

		static string backendName()
		{
			return "Custom backend";
		}

		static bool openURL(const string& url)
		{
			return false;
		}

	private:
		MediaFactory() = delete;
		MediaFactory(const MediaFactory&) = delete;
		MediaFactory(MediaFactory&&) = delete;
		MediaFactory& operator=(const MediaFactory&) = delete;
		MediaFactory& operator=(MediaFactory&&) = delete;
};
