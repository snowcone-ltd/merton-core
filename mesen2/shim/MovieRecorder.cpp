#include "Shared/Movies/MovieRecorder.h"
#include "Utilities/ZipWriter.h"

MovieRecorder::MovieRecorder(Emulator* emu)
{
}

MovieRecorder::~MovieRecorder()
{
}

void MovieRecorder::RecordInput(vector<shared_ptr<BaseControlDevice>> devices)
{
}

void MovieRecorder::OnLoadBattery(string extension, vector<uint8_t> batteryData)
{
}

vector<uint8_t> MovieRecorder::LoadBattery(string extension)
{
	return vector<uint8_t>();
}

void MovieRecorder::ProcessNotification(ConsoleNotificationType type, void *parameter)
{
}
