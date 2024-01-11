#include "SNES/Debugger/SnesPpuTools.h"
#include "Debugger/DebugTypes.h"
#include "Debugger/MemoryDumper.h"
#include "Shared/SettingTypes.h"
#include "SNES/SnesPpu.h"
#include "Shared/ColorUtilities.h"

SnesPpuTools::SnesPpuTools(Debugger* debugger, Emulator *emu) : PpuTools(debugger, emu)
{
}

void SnesPpuTools::GetPpuToolsState(BaseState& state)
{
}

void SnesPpuTools::SetPpuScanlineState(uint16_t scanline, uint8_t mode, int32_t mode7startX, int32_t mode7startY, int32_t mode7endX, int32_t mode7endY)
{
}

void SnesPpuTools::SetPpuRowBuffers(uint16_t scanline, uint16_t xStart, uint16_t xEnd, uint16_t mainScreenRowBuffer[256], uint16_t subScreenRowBuffer[256])
{
}

DebugTilemapInfo SnesPpuTools::GetTilemap(GetTilemapOptions options, BaseState& baseState, uint8_t* vram, uint32_t* palette, uint32_t* outBuffer)
{
	return {};
}

template<TileFormat format>
void SnesPpuTools::RenderTilemap(GetTilemapOptions& options, int rowCount, LayerConfig& layer, int columnCount, uint8_t* vram, int tileHeight, int tileWidth, bool largeTileHeight, bool largeTileWidth, uint8_t bpp, uint32_t* outBuffer, FrameInfo outputSize, const uint32_t* palette, uint16_t basePaletteOffset)
{
}

template<TileFormat format>
void SnesPpuTools::RenderMode7Tilemap(GetTilemapOptions& options, uint8_t* vram, uint32_t* outBuffer, const uint32_t* palette)
{
}

DebugTilemapInfo SnesPpuTools::RenderScreenView(uint8_t layer, uint32_t* outBuffer)
{
	return {};
}

void SnesPpuTools::GetSpritePreview(GetSpritePreviewOptions options, BaseState& baseState, uint8_t *vram, uint8_t *oamRam, uint32_t* palette, uint32_t* outBuffer)
{
}

void SnesPpuTools::GetSpriteInfo(DebugSpriteInfo& sprite, uint16_t spriteIndex, GetSpritePreviewOptions& options, SnesPpuState& state, uint8_t* vram, uint8_t* oamRam, uint32_t* palette)
{
}

void SnesPpuTools::GetSpriteList(GetSpritePreviewOptions options, BaseState& baseState, uint8_t* vram, uint8_t* oamRam, uint32_t* palette, DebugSpriteInfo outBuffer[])
{
}

FrameInfo SnesPpuTools::GetTilemapSize(GetTilemapOptions options, BaseState& baseState)
{
	return {};
}

DebugTilemapTileInfo SnesPpuTools::GetTilemapTileInfo(uint32_t x, uint32_t y, uint8_t* vram, GetTilemapOptions options, BaseState& baseState)
{
	return {};
}

DebugSpritePreviewInfo SnesPpuTools::GetSpritePreviewInfo(GetSpritePreviewOptions options, BaseState& baseState)
{
	return {};
}

DebugPaletteInfo SnesPpuTools::GetPaletteInfo(GetPaletteInfoOptions options)
{
	return {};
}

void SnesPpuTools::SetPaletteColor(int32_t colorIndex, uint32_t color)
{
}
