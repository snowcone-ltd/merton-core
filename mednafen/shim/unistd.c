int _open(const char *filename, int oflag, int pmode);
int _close(int fd);
int _read(int const fd, void * const buffer, unsigned const buffer_size);
int _write(int fd, const void *buffer, unsigned int count);

int open(const char *filename, int oflag, int pmode)
{
	return _open(filename, oflag, pmode);
}

int close(int fd)
{
	return _close(fd);
}

int read(int const fd, void * const buffer, unsigned const buffer_size)
{
	return _read(fd, buffer, buffer_size);
}

int write(int fd, const void *buffer, unsigned int count)
{
	return _write(fd, buffer, count);
}
