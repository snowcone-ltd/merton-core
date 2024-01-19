#include "bspf.hxx"
#include "Cart.hxx"
#include "core-fs-node.hxx"

#ifdef _WIN32
	const string SLASH = "\\";
#else
	const string SLASH = "/";
#endif

FSNodeCore::FSNodeCore() : _name{"rom"}, _path{"." + SLASH}
{
}

FSNodeCore::FSNodeCore(string_view p) : _name{p}, _path{p}
{
	if (p == "." + SLASH + "nvram")
		_path = "." + SLASH;
}

bool FSNodeCore::exists() const
{
	return true;
}

bool FSNodeCore::isReadable() const
{
	return true;
}

bool FSNodeCore::isWritable() const
{
	return true;
}

string FSNodeCore::getShortPath() const
{
	return ".";
}

bool FSNodeCore::
		getChildren(AbstractFSList& myList, ListMode mode) const
{
	return false;
}

bool FSNodeCore::makeDir()
{
	return false;
}

bool FSNodeCore::rename(string_view newfile)
{
	return false;
}

AbstractFSNodePtr FSNodeCore::getParent() const
{
	return nullptr;
}

size_t FSNodeCore::read(ByteBuffer& image, size_t) const
{
	image = make_unique<uInt8[]>(Cartridge::maxSize());

	// extern uInt32 libretro_read_rom(void* data);
	// return libretro_read_rom(image.get());
	return 0;
}
