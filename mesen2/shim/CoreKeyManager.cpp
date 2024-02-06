#include "CoreKeymanager.h"

#define MAX_PLAYERS 8

static bool BUTTONS[3][MAX_PLAYERS][CORE_BUTTON_MAX];

CoreKeyManager::CoreKeyManager(Emulator* emu)
{
}

CoreKeyManager::~CoreKeyManager()
{
}

void CoreKeyManager::RefreshState()
{
	this->m.lock();

	memcpy(BUTTONS[0], BUTTONS[1], sizeof(BUTTONS[1]));
	memcpy(BUTTONS[1], BUTTONS[2], sizeof(BUTTONS[2]));

	this->m.unlock();
}

bool CoreKeyManager::IsKeyPressed(uint16_t key)
{
	uint8_t player = (key & 0xFF00) >> 8;
	CoreButton button = (CoreButton) (key & 0xFF);

	if (player >= MAX_PLAYERS || button >= CORE_BUTTON_MAX)
		return false;

	return BUTTONS[0][player][button];
}

bool CoreKeyManager::IsMouseButtonPressed(MouseButton button)
{
	return false;
}

vector<uint16_t> CoreKeyManager::GetPressedKeys()
{
	return vector<uint16_t>();
}

string CoreKeyManager::GetKeyName(uint16_t keyCode)
{
	return "";
}

uint16_t CoreKeyManager::GetKeyCode(string keyName)
{
	return 0;
}

void CoreKeyManager::UpdateDevices()
{
}

bool CoreKeyManager::SetKeyState(uint16_t scanCode, bool state)
{
	return false;
}

void CoreKeyManager::SetDisabled(bool disabled)
{
}

void CoreKeyManager::ResetKeyState()
{
}

void CoreKeyManager::SetButton(uint8_t player, CoreButton button, bool pressed)
{
	if (player < MAX_PLAYERS) {
		this->m.lock();

		BUTTONS[1][player][button] |= pressed;
		BUTTONS[2][player][button] = pressed;

		this->m.unlock();
	}
}
