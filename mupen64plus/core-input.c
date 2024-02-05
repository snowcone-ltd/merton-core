#include "m64p_plugin.h"

#include "../core.h"

static uint16_t CORE_BUTTONS;
static int16_t AXIS_LX;
static int16_t AXIS_LY;

void input_set_button(uint8_t player, CoreButton button, bool pressed)
{
	uint16_t b = 0;

	switch (button) {
		case CORE_BUTTON_DPAD_R: b = 0x0001; break;
		case CORE_BUTTON_DPAD_L: b = 0x0002; break;
		case CORE_BUTTON_DPAD_D: b = 0x0004; break;
		case CORE_BUTTON_DPAD_U: b = 0x0008; break;
		case CORE_BUTTON_START:  b = 0x0010; break;
		case CORE_BUTTON_L2:     b = 0x0020; break;
		case CORE_BUTTON_A:      b = 0x0040; break;
		case CORE_BUTTON_B:      b = 0x0080; break;
		case CORE_BUTTON_R:      b = 0x1000; break;
		case CORE_BUTTON_L:      b = 0x2000; break;
	}

	if (pressed) {
		CORE_BUTTONS |= b;

	} else {
		CORE_BUTTONS &= ~b;
	}
}

void input_set_axis(uint8_t player, CoreAxis axis, int16_t value)
{
	int16_t cthresh = 70;
	value /= 409;

	switch (axis) {
		case CORE_AXIS_LX:
			AXIS_LX = value;
			break;
		case CORE_AXIS_LY:
			AXIS_LY = value;
			break;
		case CORE_AXIS_RX:
			if (value < -cthresh) {
				CORE_BUTTONS |= 0x0200;

			} else if (value > cthresh) {
				CORE_BUTTONS |= 0x0100;

			} else {
				CORE_BUTTONS &= ~0x0100;
				CORE_BUTTONS &= ~0x0200;
			}
			break;
		case CORE_AXIS_RY:
			if (value < -cthresh) {
				CORE_BUTTONS |= 0x0400;

			} else if (value > cthresh) {
				CORE_BUTTONS |= 0x0800;

			} else {
				CORE_BUTTONS &= ~0x0400;
				CORE_BUTTONS &= ~0x0800;
			}
			break;
	}
}

EXPORT m64p_error INPUT_PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion,
	const char **PluginNamePtr, int *Capabilities)
{
	if (PluginType)
		*PluginType = M64PLUGIN_INPUT;

	if (PluginVersion)
		*PluginVersion = 0x010000;

	if (APIVersion)
		*APIVersion = 0x020100;

	if (PluginNamePtr)
		*PluginNamePtr = "Core Input";

	if (Capabilities)
		*Capabilities = 0;

	return M64ERR_SUCCESS;
}

EXPORT void INPUT_ControllerCommand(int Control, unsigned char *Command)
{
	// TODO Rumble
}

EXPORT void INPUT_GetKeys(int Control, BUTTONS *Keys)
{
	if (Control != 0)
		return;

	Keys->Value = CORE_BUTTONS;
	Keys->X_AXIS = AXIS_LX;
	Keys->Y_AXIS = AXIS_LY;
}

EXPORT void INPUT_InitiateControllers(CONTROL_INFO ControlInfo)
{
	ControlInfo.Controls[0].Present = 1;
}

EXPORT void INPUT_ReadController(int Control, unsigned char *Command)
{
}

EXPORT void INPUT_RomClosed(void)
{
}

EXPORT int INPUT_RomOpen(void)
{
	return 1;
}

EXPORT void INPUT_SDL_KeyDown(int keymod, int keysym)
{
}

EXPORT void INPUT_SDL_KeyUp(int keymod, int keysym)
{
}

EXPORT void INPUT_RenderCallback(void)
{
}

EXPORT void INPUT_SendVRUWord(uint16_t length, uint16_t *word, uint8_t lang)
{
}

EXPORT void INPUT_SetMicState(int state)
{
}

EXPORT void INPUT_ReadVRUResults(uint16_t *error_flags, uint16_t *num_results, uint16_t *mic_level,
	uint16_t *voice_level, uint16_t *voice_length, uint16_t *matches)
{
}

EXPORT void INPUT_ClearVRUWords(uint8_t length)
{
}

EXPORT void INPUT_SetVRUWordMask(uint8_t length, uint8_t *mask)
{
}
