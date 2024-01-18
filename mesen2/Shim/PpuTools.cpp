#include "Debugger/PpuTools.h"
#include "Debugger/CdlManager.h"
#include "Debugger/DebugTypes.h"
#include "Debugger/DebugBreakHelper.h"
#include "Shared/SettingTypes.h"

PpuTools::PpuTools(Debugger* debugger, Emulator *emu)
{
}

void PpuTools::BlendColors(uint8_t output[4], uint8_t input[4])
{
}

void PpuTools::GetTileView(GetTileViewOptions options, uint8_t* source, uint32_t srcSize, const uint32_t* colors, uint32_t* outBuffer)
{
}

uint32_t PpuTools::GetBackgroundColor(TileBackground bgColor, const uint32_t* colors, uint8_t paletteIndex, uint8_t bpp)
{
	return 0;
}

uint32_t PpuTools::GetSpriteBackgroundColor(SpriteBackground bgColor, const uint32_t* colors, bool useDarkerColor)
{
	return 0;
}

template<TileFormat format>
void PpuTools::InternalGetTileView(GetTileViewOptions options, uint8_t *source, uint32_t srcSize, const uint32_t *colors, uint32_t *outBuffer)
{
}

bool PpuTools::IsTileHidden(MemoryType memType, uint32_t addr, GetTileViewOptions& options)
{
	return false;
}

void PpuTools::SetViewerUpdateTiming(uint32_t viewerId, uint16_t scanline, uint16_t cycle)
{
}

void PpuTools::RemoveViewer(uint32_t viewerId)
{
}

int32_t PpuTools::GetTilePixel(AddressInfo tileAddress, TileFormat format, int32_t x, int32_t y)
{
	return 0;
}

void PpuTools::SetTilePixel(AddressInfo tileAddress, TileFormat format, int32_t x, int32_t y, int32_t color)
{
}

void PpuTools::GetSetTilePixel(AddressInfo tileAddress, TileFormat format, int32_t x, int32_t y, int32_t& color, bool forGet)
{
}

void PpuTools::UpdateViewers(uint16_t scanline, uint16_t cycle)
{
}
