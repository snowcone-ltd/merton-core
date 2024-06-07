#include "util/image.h"

RGBA8Image::RGBA8Image() = default;

RGBA8Image::RGBA8Image(const RGBA8Image& copy) : Image(copy)
{
}

RGBA8Image::RGBA8Image(u32 width, u32 height, const u32* pixels) : Image(width, height, pixels)
{
}

RGBA8Image::RGBA8Image(RGBA8Image&& move) : Image(move)
{
}

RGBA8Image::RGBA8Image(u32 width, u32 height) : Image(width, height)
{
}

RGBA8Image::RGBA8Image(u32 width, u32 height, std::vector<u32> pixels) : Image(width, height, std::move(pixels))
{
}

RGBA8Image& RGBA8Image::operator=(const RGBA8Image& copy)
{
	Image<u32>::operator=(copy);
	return *this;
}

RGBA8Image& RGBA8Image::operator=(RGBA8Image&& move)
{
	Image<u32>::operator=(move);
	return *this;
}

bool RGBA8Image::LoadFromFile(const char* filename)
{
	return false;
}

bool RGBA8Image::SaveToFile(const char* filename, u8 quality) const
{
	return false;
}

bool RGBA8Image::LoadFromFile(std::string_view filename, std::FILE* fp)
{
	return false;
}

bool RGBA8Image::LoadFromBuffer(std::string_view filename, const void* buffer, size_t buffer_size)
{
	return false;
}

bool RGBA8Image::SaveToFile(std::string_view filename, std::FILE* fp, u8 quality) const
{
	return false;
}

std::optional<std::vector<u8>> RGBA8Image::SaveToBuffer(std::string_view filename, u8 quality) const
{
	std::optional<std::vector<u8>> ret;

	return ret;
}
