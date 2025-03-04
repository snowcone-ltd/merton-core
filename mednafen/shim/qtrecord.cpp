#include <mednafen/mednafen.h>
#include <mednafen/FileStream.h>
#include "resampler/resampler.h"
#include <mednafen/Time.h>
#include "qtrecord.h"
#include <minilzo/minilzo.h>
#include "video/png.h"

namespace Mednafen
{

void QTRecord::w16(uint16 val)
{
}

void QTRecord::w32(uint32 val)
{
}

void QTRecord::w32s(const char *str)
{
}

void QTRecord::w64s(const char *str)
{
}


void QTRecord::w64(uint64 val)
{
}

// fixed_len doesn't include the leading 1-byte count
void QTRecord::wps(const char *str, uint8 fixed_len)
{
}

void QTRecord::vardata_begin(void)
{
}

void QTRecord::vardata_end(void)
{
}

void QTRecord::atom_begin(uint32 type, bool small_atom)
{
}


void QTRecord::atom_begin(const char *type, bool small_atom)
{
}

void QTRecord::atom_end(void)
{
}

QTRecord::QTRecord(const std::string& path, const VideoSpec &spec) : qtfile(path, FileStream::MODE_WRITE_SAFE), resampler(NULL)
{
}


template<typename T, uint64 rgb16_tag = 0>
static void DecodeLine(const T* src, const MDFN_PixelFormat& src_pf, int src_width, uint8* dest, bool dest_bgr, uint32 dest_width)
{
}

void QTRecord::WriteFrame(const MDFN_Surface *surface, const MDFN_Rect &DisplayRect, const int32 *LineWidths,
			  const int16 *SoundBuf, const int32 SoundBufSize, const int64 MasterCycles)
{
}

void QTRecord::Write_ftyp(void) // Leaf
{
}

void QTRecord::Write_mvhd(void)	// Leaf
{
}

void QTRecord::Write_tkhd(void)	// Leaf
{
}

// Sample description
void QTRecord::Write_stsd(void) // Leaf
{
}

// Time-to-sample
void QTRecord::Write_stts(void)	// Leaf
{
}

// Sample-to-chunk
void QTRecord::Write_stsc(void) // Leaf
{
}

void QTRecord::Write_stsz(void) // Leaf
{
}

// Chunk offset atom(64-bit style)
void QTRecord::Write_co64(void) // Leaf
{
}

void QTRecord::Write_stco(void) // Leaf
{
}

void QTRecord::Write_stbl(void)
{
}

// Media header atom
void QTRecord::Write_mdhd(void)	// Leaf
{
}

// Sound media information header
void QTRecord::Write_smhd(void) // Leaf
{
}

// Video media information header
void QTRecord::Write_vmhd(void) // Leaf
{
}

void QTRecord::Write_hdlr(const char *str, const char *comp_name) // Leaf
{
}

void QTRecord::Write_dinf(void)
{
}

void QTRecord::Write_minf(void)
{
}

void QTRecord::Write_mdia(void)
{
}

void QTRecord::Write_edts(void)
{
}

void QTRecord::Write_trak(void)
{
}

void QTRecord::Write_udta(void)
{
}

void QTRecord::Write_moov(void)
{
}

void QTRecord::Finish(void)
{
}

QTRecord::~QTRecord(void)
{
}

}
