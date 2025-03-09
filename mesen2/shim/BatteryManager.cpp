#include "Shared/BatteryManager.h"

void core_log(const char *fmt, ...);

static uint8_t *BATTERY_MANAGER_SAVE_DATA;
static size_t BATTERY_MANAGER_SAVE_DATA_SIZE;
static bool BATTERY_MANAGER_SAVE_DATA_DIRTY;

void battery_manager_set_save_data(const void *save_data, size_t save_data_size)
{
	if (save_data_size == BATTERY_MANAGER_SAVE_DATA_SIZE &&
		memcmp(save_data, BATTERY_MANAGER_SAVE_DATA, save_data_size) == 0)
	{
		return;
	}

	free(BATTERY_MANAGER_SAVE_DATA);
	BATTERY_MANAGER_SAVE_DATA = NULL;
	BATTERY_MANAGER_SAVE_DATA_SIZE = 0;

	if (save_data) {
		BATTERY_MANAGER_SAVE_DATA = (uint8_t *) malloc(save_data_size);
		memcpy(BATTERY_MANAGER_SAVE_DATA, save_data, save_data_size);

		BATTERY_MANAGER_SAVE_DATA_SIZE = save_data_size;
		BATTERY_MANAGER_SAVE_DATA_DIRTY = true;

		core_log("[SDATA] Saved, size=%zu\n", save_data_size);
	}
}

const void *battery_manager_get_save_data(size_t *save_data_size)
{
	if (!BATTERY_MANAGER_SAVE_DATA_DIRTY)
		return NULL;

	BATTERY_MANAGER_SAVE_DATA_DIRTY = false;

	if (save_data_size)
		*save_data_size = BATTERY_MANAGER_SAVE_DATA_SIZE;

	return BATTERY_MANAGER_SAVE_DATA;
}

void BatteryManager::Initialize(string romName, bool setBatteryFlag)
{
}

void BatteryManager::SetBatteryProvider(shared_ptr<IBatteryProvider> provider)
{
}

void BatteryManager::SetBatteryRecorder(shared_ptr<IBatteryRecorder> recorder)
{
}

void BatteryManager::SaveBattery(string extension, uint8_t* data, uint32_t length)
{
	battery_manager_set_save_data(data, length);
}

vector<uint8_t> BatteryManager::LoadBattery(string extension)
{
	if (BATTERY_MANAGER_SAVE_DATA) {
		core_log("[SDATA] Read, size=%zu\n", BATTERY_MANAGER_SAVE_DATA_SIZE);

		return vector<uint8_t>(BATTERY_MANAGER_SAVE_DATA, BATTERY_MANAGER_SAVE_DATA +
			BATTERY_MANAGER_SAVE_DATA_SIZE);
	}

	return vector<uint8_t>();
}

void BatteryManager::LoadBattery(string extension, uint8_t* data, uint32_t length)
{
	if (BATTERY_MANAGER_SAVE_DATA && length <= BATTERY_MANAGER_SAVE_DATA_SIZE) {
		memcpy(data, BATTERY_MANAGER_SAVE_DATA, length);
		core_log("[SDATA] Read, size=%zu\n", BATTERY_MANAGER_SAVE_DATA_SIZE);
	}
}

uint32_t BatteryManager::GetBatteryFileSize(string extension)
{
	return 0;
}
