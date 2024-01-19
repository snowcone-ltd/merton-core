#include "Shared/SaveStateManager.h"

#include "Utilities/FolderUtilities.h"
#include "Utilities/ZipWriter.h"
#include "Utilities/ZipReader.h"
#include "Utilities/PNGHelper.h"
#include "Shared/MessageManager.h"
#include "Shared/Emulator.h"
#include "Shared/EmuSettings.h"
#include "Shared/Movies/MovieManager.h"
#include "Shared/RenderedFrame.h"
#include "Shared/EventType.h"
#include "Debugger/Debugger.h"
#include "Netplay/GameClient.h"
#include "Shared/Video/VideoDecoder.h"
#include "Shared/Video/VideoRenderer.h"
#include "Shared/Video/BaseVideoFilter.h"

SaveStateManager::SaveStateManager(Emulator* emu)
{
}

string SaveStateManager::GetStateFilepath(int stateIndex)
{
	return "";
}

void SaveStateManager::SelectSaveSlot(int slotIndex)
{
}

void SaveStateManager::MoveToNextSlot()
{
}

void SaveStateManager::MoveToPreviousSlot()
{
}

void SaveStateManager::SaveState()
{
}

bool SaveStateManager::LoadState()
{
	return false;
}

void SaveStateManager::GetSaveStateHeader(ostream &stream)
{
}

void SaveStateManager::SaveState(ostream &stream)
{
}

bool SaveStateManager::SaveState(string filepath, bool showSuccessMessage)
{
	return false;
}

void SaveStateManager::SaveState(int stateIndex, bool displayMessage)
{
}

void SaveStateManager::SaveVideoData(ostream& stream)
{
}

bool SaveStateManager::GetVideoData(vector<uint8_t>& out, RenderedFrame& frame, istream& stream)
{
	return false;
}

bool SaveStateManager::LoadState(istream &stream)
{
	return false;
}

bool SaveStateManager::LoadState(string filepath, bool showSuccessMessage)
{
	return false;
}

bool SaveStateManager::LoadState(int stateIndex)
{
	return false;
}

void SaveStateManager::SaveRecentGame(string romName, string romPath, string patchPath)
{
}

void SaveStateManager::LoadRecentGame(string filename, bool resetGame)
{
}

int32_t SaveStateManager::GetSaveStatePreview(string saveStatePath, uint8_t* pngData)
{
	return -1;
}

void SaveStateManager::WriteValue(ostream& stream, uint32_t value)
{
}

uint32_t SaveStateManager::ReadValue(istream& stream)
{
	return 0;
}
