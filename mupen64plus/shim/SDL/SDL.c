#include "SDL.h"

#include <math.h>
#include <stdlib.h>

#if defined(_WIN32)
	#include <windows.h>
#else
	#include <time.h>
	#include <pthread.h>
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
	ms = 1; // XXX Increase precision

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

struct SDL_Thread {
	SDL_ThreadFunction func;
	void *data;
	int ret;

	#if defined(_WIN32)
		HANDLE h;
	#else
		pthread_t t;
	#endif
};

#if defined(_WIN32)
static DWORD WINAPI thread_func(LPVOID *lpParameter)
{
	SDL_Thread *ctx = (SDL_Thread *) lpParameter;

	ctx->ret = ctx->func(ctx->data);

	return 0;
}
#else
static void *thread_func(void *data)
{
	SDL_Thread *ctx = data;

	ctx->ret = ctx->func(ctx->data);

	return NULL;
}
#endif

SDL_Thread *SDL_CreateThread(SDL_ThreadFunction fn, const char *name, void *data)
{
	SDL_Thread *ctx = calloc(1, sizeof(SDL_Thread));
	ctx->func = fn;
	ctx->data = data;

	#if defined(_WIN32)
		ctx->h = CreateThread(NULL, 0, thread_func, ctx, 0, NULL);
	#else
		pthread_create(&ctx->t, NULL, thread_func, ctx);
	#endif

	return ctx;
}

void SDL_WaitThread(SDL_Thread *ctx, int *status)
{
	if (!ctx)
		return;

	#if defined(_WIN32)
	if (ctx->h) {
		WaitForSingleObject(ctx->h, INFINITE);
		CloseHandle(ctx->h);
	}

	#else
	if (ctx->t)
		pthread_join(ctx->t, NULL);
	#endif

	if (status)
		*status = ctx->ret;

	free(ctx);
}

SDL_Mutex *SDL_CreateMutex(void)
{
	#if defined(_WIN32)
	CRITICAL_SECTION *mutex = calloc(1, sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(mutex);
	#else
	pthread_mutex_t *mutex = calloc(1, sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);
	#endif

	return (SDL_Mutex *) mutex;
}

void SDL_DestroyMutex(SDL_Mutex *mutex)
{
	if (!mutex)
		return;

	#if defined(_WIN32)
	DeleteCriticalSection((CRITICAL_SECTION *) mutex);
	#else
	pthread_mutex_destroy((pthread_mutex_t *) mutex);
	#endif

	free(mutex);
}

int SDL_LockMutex(SDL_Mutex *mutex)
{
	#if defined(_WIN32)
	EnterCriticalSection((CRITICAL_SECTION *) mutex);
	#else
	pthread_mutex_lock((pthread_mutex_t *) mutex);
	#endif

	return 0;
}

int SDL_UnlockMutex(SDL_Mutex *mutex)
{
	#if defined(_WIN32)
	LeaveCriticalSection((CRITICAL_SECTION *) mutex);
	#else
	pthread_mutex_unlock((pthread_mutex_t *) mutex);
	#endif

	return 0;
}

SDL_Condition *SDL_CreateCondition(void)
{
	#if defined(_WIN32)
	CONDITION_VARIABLE *cond = calloc(1, sizeof(CONDITION_VARIABLE));
	InitializeConditionVariable(cond);
	#else
	pthread_cond_t *cond = calloc(1, sizeof(pthread_cond_t));
	pthread_cond_init(cond, NULL);
	#endif

	return (SDL_Condition *) cond;
}

void SDL_DestroyCondition(SDL_Condition *cond)
{
	if (!cond)
		return;

	#if !defined(_WIN32)
	pthread_cond_destroy((pthread_cond_t *) cond);
	#endif

	free(cond);
}

int SDL_SignalCondition(SDL_Condition *cond)
{
	#if defined(_WIN32)
	WakeConditionVariable((CONDITION_VARIABLE *) cond);
	#else
	pthread_cond_signal((pthread_cond_t *) cond);
	#endif

	return 0;
}

int SDL_WaitCondition(SDL_Condition *cond, SDL_Mutex *mutex)
{
	#if defined(_WIN32)
	SleepConditionVariableCS((CONDITION_VARIABLE *) cond, (CRITICAL_SECTION *) mutex, INFINITE);
	#else
	pthread_cond_wait((pthread_cond_t *) cond, (pthread_mutex_t *) mutex);
	#endif

	return 0;
}
