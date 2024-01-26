#include "SDL.h"

#include <math.h>

#if defined(_WIN32)
	#include <windows.h>
#else
	#include <time.h>
#endif

void core_log(const char *fmt, ...);

void SDL_Quit(void)
{
}

uint32_t SDL_WasInit(uint32_t flags)
{
	return 0xFFFFFFFF;
}

void SDL_PumpEvents(void)
{
}

void SDL_SetEventFilter(SDL_EventFilter filter, void *userdata)
{
}

void SDL_Delay(uint32_t ms)
{
	#if defined(_WIN32)
		Sleep(ms);

	#else
		struct timespec ts = {
			.tv_sec = ms / 1000,
			.tv_nsec = (ms % 1000) * 1000 * 1000,
		};

		nanosleep(&ts, NULL);
	#endif
}

uint32_t SDL_GetTicks(void)
{
	#if defined(_WIN32)
		LARGE_INTEGER ts;
		QueryPerformanceCounter(&ts);

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		return ts.QuadPart / (frequency.QuadPart / 1000);

	#else
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

		return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	#endif
}

SDL_mutex *SDL_CreateMutex(void)
{
	return (SDL_mutex *) (uintptr_t) 1;
}

void SDL_DestroyMutex(SDL_mutex *mutex)
{
}

int SDL_LockMutex(SDL_mutex *mutex)
{
	return 0;
}

int SDL_UnlockMutex(SDL_mutex *mutex)
{
	return 0;
}
