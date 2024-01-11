#include "ShimKeyManager.h"

#define MAX_PLAYERS 8

static bool BUTTONS[3][MAX_PLAYERS][CORE_BUTTON_MAX];

ShimKeyManager::ShimKeyManager(Emulator* emu)
{
}

ShimKeyManager::~ShimKeyManager()
{
}

void ShimKeyManager::RefreshState()
{
	this->m.lock();

	memcpy(BUTTONS[0], BUTTONS[1], sizeof(BUTTONS[1]));
	memcpy(BUTTONS[1], BUTTONS[2], sizeof(BUTTONS[2]));

	this->m.unlock();
}

bool ShimKeyManager::IsKeyPressed(uint16_t key)
{
	uint8_t player = (key & 0xFF00) >> 8;
	CoreButton button = (CoreButton) (key & 0xFF);

	if (player >= MAX_PLAYERS || button >= CORE_BUTTON_MAX)
		return false;

	return BUTTONS[0][player][button];
}

bool ShimKeyManager::IsMouseButtonPressed(MouseButton button)
{
	return false;
}

vector<uint16_t> ShimKeyManager::GetPressedKeys()
{
	return vector<uint16_t>();
}

string ShimKeyManager::GetKeyName(uint16_t keyCode)
{
	return "";
}

uint16_t ShimKeyManager::GetKeyCode(string keyName)
{
	return 0;
}

void ShimKeyManager::UpdateDevices()
{
}

bool ShimKeyManager::SetKeyState(uint16_t scanCode, bool state)
{
	return false;
}

void ShimKeyManager::SetDisabled(bool disabled)
{
}

void ShimKeyManager::ResetKeyState()
{
}

void ShimKeyManager::SetButton(uint8_t player, CoreButton button, bool pressed)
{
	if (player < MAX_PLAYERS) {
		this->m.lock();

		BUTTONS[1][player][button] |= pressed;
		BUTTONS[2][player][button] = pressed;

		this->m.unlock();
	}
}
