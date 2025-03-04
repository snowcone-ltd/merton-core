#include "mednafen.h"

#include <zlib.h>
#include <trio/trio.h>

#include <map>

#include "netplay.h"
#include "netplay-driver.h"
#include "general.h"
#include <mednafen/string/string.h>
#include "state.h"
#include "movie.h"
#include <mednafen/hash/md5.h>
#include <mednafen/Time.h>
#include <mednafen/net/Net.h>
#include "mempatcher.h"

#include "MemoryStream.h"

#include "driver.h"

namespace Mednafen
{

int MDFNnetplay=0;

bool NetplaySendCommand(uint8 cmd, uint32 len, const void* data)
{
	return false;
}

void NetplaySendState(void)
{
}

void Netplay_Update(const uint32 PortDevIdx[], uint8* const PortData[], const uint32 PortLen[])
{
}

void Netplay_PostProcess(const uint32 PortDevIdx[], uint8* const PortData[], const uint32 PortLen[])
{
}

void MDFNI_NetplayDisconnect(void)
{
}

void MDFNI_NetplayConnect(void)
{
}

void MDFNI_NetplayLine(const char *text, bool &inputable, bool &viewable)
{
}

}
