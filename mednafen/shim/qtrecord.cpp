#include "mednafen.h"
#include "qtrecord.h"

using namespace Mednafen;

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

void QTRecord::WriteFrame(const MDFN_Surface *surface, const MDFN_Rect &DisplayRect, const int32 *LineWidths,
			  const int16 *SoundBuf, const int32 SoundBufSize, const int64 MasterCycles)
{
}

void QTRecord::Write_ftyp(void)
{
}

void QTRecord::Write_mvhd(void)
{
}

void QTRecord::Write_tkhd(void)
{
}

void QTRecord::Write_stsd(void)
{
}

void QTRecord::Write_stts(void)
{
}

void QTRecord::Write_stsc(void)
{
}

void QTRecord::Write_stsz(void)
{
}

void QTRecord::Write_co64(void)
{
}

void QTRecord::Write_stco(void)
{
}

void QTRecord::Write_stbl(void)
{
}

void QTRecord::Write_mdhd(void)
{
}

void QTRecord::Write_smhd(void)
{
}

void QTRecord::Write_vmhd(void)
{
}

void QTRecord::Write_hdlr(const char *str, const char *comp_name)
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
