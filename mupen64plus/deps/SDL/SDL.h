#pragma once

#include <stdint.h>

#define SDLCALL
#define SDL_VERSION_ATLEAST(X, Y, Z) \
	((2 >= X) && (2 > X || 28 >= Y) && (2 > X || 28 > Y || 5 >= Z))

typedef enum {
    SDL_WINDOWEVENT_MOVED,
    SDL_WINDOWEVENT_RESIZED,
} SDL_WindowEventID;

typedef enum {
	SDL_SCANCODE_DUMMY,
} SDL_Scancode;

typedef enum {
	SDL_QUIT,
	SDL_KEYDOWN,
	SDL_KEYUP,
	SDL_WINDOWEVENT,
} SDL_EventType;

typedef union {
	uint32_t type;
	struct {
		uint32_t type;
		uint8_t repeat;
		struct {
			SDL_Scancode scancode;
			uint16_t mod;
		} keysym;
	} key;
	struct {
		uint32_t type;
		int32_t data1;
		int32_t data2;
		uint8_t event;
	} window;
} SDL_Event;

typedef int (*SDL_EventFilter)(void *userdata, SDL_Event *event);
typedef int32_t SDL_JoystickID;
typedef struct SDL_mutex SDL_mutex;

void SDL_Quit(void);
uint32_t SDL_WasInit(uint32_t flags);

void SDL_PumpEvents(void);
void SDL_SetEventFilter(SDL_EventFilter filter, void *userdata);

void SDL_Delay(uint32_t ms);
uint32_t SDL_GetTicks(void);

SDL_mutex *SDL_CreateMutex(void);
void SDL_DestroyMutex(SDL_mutex *mutex);
int SDL_LockMutex(SDL_mutex *mutex);
int SDL_UnlockMutex(SDL_mutex *mutex);
