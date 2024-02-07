// Defining __APPLE__ queries the kernel for page size, important
// for the Raspberry Pi which uses 16KB
#if defined(__aarch64__)
	#define __APPLE__
#endif

#include "../../parallel-rsp/jit_allocator.cpp"
