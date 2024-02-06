#pragma once

#include <mutex>

#include "Core/Shared/Interfaces/IKeyManager.h"

#include "core.h"

class Emulator;

class CoreKeyManager : public IKeyManager
{
private:
	std::mutex m;

public:
	CoreKeyManager(Emulator* emu);
	~CoreKeyManager();

	void RefreshState();
	bool IsKeyPressed(uint16_t key);
	bool IsMouseButtonPressed(MouseButton button);
	vector<uint16_t> GetPressedKeys();
	string GetKeyName(uint16_t key);
	uint16_t GetKeyCode(string keyName);

	bool SetKeyState(uint16_t scanCode, bool state);
	void ResetKeyState();
	void SetDisabled(bool disabled);

	void UpdateDevices();

	void SetButton(uint8_t player, CoreButton button, bool pressed);
};
