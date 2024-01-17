#include "SDL.h"

#include <stdlib.h>
#include <windows.h>
#include <math.h>

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
	Sleep(ms);
}

uint32_t SDL_GetTicks(void)
{
	LARGE_INTEGER ts;
	QueryPerformanceCounter(&ts);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	return lrint((double) ts.QuadPart / (frequency.QuadPart / 1000.0));
}

SDL_mutex *SDL_CreateMutex(void)
{
	SDL_mutex *m = malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection((CRITICAL_SECTION *) m);

	return m;
}

void SDL_DestroyMutex(SDL_mutex *mutex)
{
	DeleteCriticalSection((CRITICAL_SECTION *) mutex);
	free(mutex);
}

int SDL_LockMutex(SDL_mutex *mutex)
{
	EnterCriticalSection((CRITICAL_SECTION *) mutex);

	return 0;
}

int SDL_UnlockMutex(SDL_mutex *mutex)
{
	LeaveCriticalSection((CRITICAL_SECTION *) mutex);

	return 0;
}
