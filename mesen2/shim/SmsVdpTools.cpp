#include "SMS/Debugger/SmsVdpTools.h"
#include "SMS/SmsTypes.h"
#include "SMS/SmsConsole.h"
#include "SMS/SmsVdp.h"
#include "Debugger/DebugTypes.h"
#include "Debugger/MemoryDumper.h"
#include "Shared/SettingTypes.h"

SmsVdpTools::SmsVdpTools(Debugger* debugger, Emulator *emu, SmsConsole* console) : PpuTools(debugger, emu)
{
}

void SmsVdpTools::SetMemoryAccessData(uint16_t scanline, SmsVdpMemAccess* data, uint16_t scanlineCount)
{
}

FrameInfo SmsVdpTools::GetTilemapSize(GetTilemapOptions options, BaseState& baseState)
{
	return {};
}

DebugTilemapInfo SmsVdpTools::GetTilemap(GetTilemapOptions options, BaseState& baseState, BaseState& ppuToolsState, uint8_t* vram, uint32_t* palette, uint32_t* outBuffer)
{
	return {};
}

DebugTilemapTileInfo SmsVdpTools::GetTilemapTileInfo(uint32_t x, uint32_t y, uint8_t* vram, GetTilemapOptions options, BaseState& baseState, BaseState& ppuToolsState)
{
	return {};
}

void SmsVdpTools::GetSpritePreview(GetSpritePreviewOptions options, BaseState& baseState, DebugSpriteInfo* sprites, uint32_t* spritePreviews, uint32_t* palette, uint32_t* outBuffer)
{
}

DebugSpritePreviewInfo SmsVdpTools::GetSpritePreviewInfo(GetSpritePreviewOptions options, BaseState& baseState, BaseState& ppuToolsState)
{
	return {};
}

void SmsVdpTools::GetSpriteInfo(DebugSpriteInfo& sprite, uint32_t* spritePreview, uint16_t i, GetSpritePreviewOptions& options, SmsVdpState& state, uint8_t* vram, uint8_t* oamRam, uint32_t* palette)
{
}

void SmsVdpTools::GetSpriteList(GetSpritePreviewOptions options, BaseState& baseState, BaseState& ppuToolsState, uint8_t* vram, uint8_t* oamRam, uint32_t* palette, DebugSpriteInfo outBuffer[], uint32_t* spritePreviews, uint32_t* screenPreview)
{
}

DebugPaletteInfo SmsVdpTools::GetPaletteInfo(GetPaletteInfoOptions options)
{
	return {};
}

void SmsVdpTools::SetPaletteColor(int32_t colorIndex, uint32_t color)
{
}
