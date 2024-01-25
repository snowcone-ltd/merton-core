#include "SDL.h"

#include "matoya.h"

static MTY_Time TS;

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
	MTY_Sleep(ms);
}

uint32_t SDL_GetTicks(void)
{
	if (TS == 0)
		TS = MTY_GetTime();

	return lrint(MTY_TimeDiff(TS, MTY_GetTime()));
}

SDL_mutex *SDL_CreateMutex(void)
{
	return (SDL_mutex *) MTY_MutexCreate();
}

void SDL_DestroyMutex(SDL_mutex *mutex)
{
	MTY_Mutex *m = (MTY_Mutex *) mutex;

	MTY_MutexDestroy(&m);
}

int SDL_LockMutex(SDL_mutex *mutex)
{
	MTY_MutexLock((MTY_Mutex *) mutex);

	return 0;
}

int SDL_UnlockMutex(SDL_mutex *mutex)
{
	MTY_MutexUnlock((MTY_Mutex *) mutex);

	return 0;
}
