#include <stdint.h>

#include "util/sockets.h"

namespace GDBServer {
	bool Initialize(uint16_t port) {return false;}
	bool HasAnyClients() {return false;}
	void Shutdown() {}
	void OnSystemPaused() {}
	void OnSystemResumed() {}
}

namespace PINEServer {
	bool IsRunning() {return false;}
	bool Initialize(uint16_t slot) {return false;}
	void Shutdown() {}
}

SocketMultiplexer::SocketMultiplexer() = default;

SocketMultiplexer::~SocketMultiplexer()
{
}

std::unique_ptr<SocketMultiplexer> SocketMultiplexer::Create(Error* error)
{
	return std::unique_ptr<SocketMultiplexer>(new SocketMultiplexer());
}

bool SocketMultiplexer::PollEventsWithTimeout(u32 milliseconds)
{
	return false;
}

bool SocketMultiplexer::HasAnyOpenSockets()
{
	return false;
}

#include "core/system.cpp"
