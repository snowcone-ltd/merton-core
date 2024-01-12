PROJ = mgba
NAME = mgba

CORE_OBJS = \
	$(PROJ)/src/core/bitmap-cache.obj \
	$(PROJ)/src/core/cache-set.obj \
	$(PROJ)/src/core/cheats.obj \
	$(PROJ)/src/core/config.obj \
	$(PROJ)/src/core/core.obj \
	$(PROJ)/src/core/interface.obj \
	$(PROJ)/src/core/log.obj \
	$(PROJ)/src/core/map-cache.obj \
	$(PROJ)/src/core/serialize.obj \
	$(PROJ)/src/core/sync.obj \
	$(PROJ)/src/core/tile-cache.obj \
	$(PROJ)/src/core/timing.obj

UTIL_OBJS = \
	$(PROJ)/src/util/vfs/vfs-fd.obj \
	$(PROJ)/src/util/vfs/vfs-mem.obj \
	$(PROJ)/src/util/circle-buffer.obj \
	$(PROJ)/src/util/configuration.obj \
	$(PROJ)/src/util/crc32.obj \
	$(PROJ)/src/util/formatting.obj \
	$(PROJ)/src/util/gbk-table.obj \
	$(PROJ)/src/util/hash.obj \
	$(PROJ)/src/util/memory.obj \
	$(PROJ)/src/util/patch.obj \
	$(PROJ)/src/util/patch-ips.obj \
	$(PROJ)/src/util/patch-ups.obj \
	$(PROJ)/src/util/string.obj \
	$(PROJ)/src/util/table.obj \
	$(PROJ)/src/util/vector.obj \
	$(PROJ)/src/util/vfs.obj

THIRD_PARTY_OBJS = \
	$(PROJ)/src/third-party/blip_buf/blip_buf.obj \
	$(PROJ)/src/third-party/inih/ini.obj

GB_OBJS = \
	$(PROJ)/src/gb/audio.obj

ARM_OBJS = \
	$(PROJ)/src/arm/arm.obj \
	$(PROJ)/src/arm/decoder-arm.obj \
	$(PROJ)/src/arm/decoder-thumb.obj \
	$(PROJ)/src/arm/isa-arm.obj \
	$(PROJ)/src/arm/isa-thumb.obj

GBA_OBJS = \
	$(PROJ)/src/gba/cart/ereader.obj \
	$(PROJ)/src/gba/cart/gpio.obj \
	$(PROJ)/src/gba/cart/matrix.obj \
	$(PROJ)/src/gba/cart/vfame.obj \
	$(PROJ)/src/gba/cheats/codebreaker.obj \
	$(PROJ)/src/gba/cheats/gameshark.obj \
	$(PROJ)/src/gba/cheats/parv3.obj \
	$(PROJ)/src/gba/renderers/cache-set.obj \
	$(PROJ)/src/gba/renderers/common.obj \
	$(PROJ)/src/gba/renderers/software-bg.obj \
	$(PROJ)/src/gba/renderers/software-mode0.obj \
	$(PROJ)/src/gba/renderers/software-obj.obj \
	$(PROJ)/src/gba/renderers/video-software.obj \
	$(PROJ)/src/gba/sio/gbp.obj \
	$(PROJ)/src/gba/audio.obj \
	$(PROJ)/src/gba/bios.obj \
	$(PROJ)/src/gba/cheats.obj \
	$(PROJ)/src/gba/core.obj \
	$(PROJ)/src/gba/dma.obj \
	$(PROJ)/src/gba/gba.obj \
	$(PROJ)/src/gba/hle-bios.obj \
	$(PROJ)/src/gba/io.obj \
	$(PROJ)/src/gba/memory.obj \
	$(PROJ)/src/gba/overrides.obj \
	$(PROJ)/src/gba/savedata.obj \
	$(PROJ)/src/gba/serialize.obj \
	$(PROJ)/src/gba/sio.obj \
	$(PROJ)/src/gba/timer.obj \
	$(PROJ)/src/gba/video.obj

BASE_OBJS = \
	$(CORE_OBJS) \
	$(THIRD_PARTY_OBJS) \
	$(UTIL_OBJS) \
	$(GB_OBJS)

EMU_OBJS = \
	$(ARM_OBJS) \
	$(GBA_OBJS)

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4005 \
	/wd4018 \
	/wd4057 \
	/wd4098 \
	/wd4100 \
	/wd4127 \
	/wd4132 \
	/wd4133 \
	/wd4146 \
	/wd4201 \
	/wd4244 \
	/wd4245 \
	/wd4267 \
	/wd4273 \
	/wd4295 \
	/wd4334 \
	/wd4389 \
	/wd4456 \
	/wd4457 \
	/wd4701 \
	/wd4702 \
	/wd4706 \
	/wd4996 \
	/volatile:iso \
	/std:c11 \
	/nologo

INCLUDES = \
	-I$(PROJ)/include \
	-I$(PROJ)/src

DEFS = \
	-DUNICODE \
	-D_CRT_SECURE_NO_WARNINGS \
	-DDISABLE_THREADING \
	-DCOLOR_16_BIT \
	-DCOLOR_5_6_5 \
	-DMINIMAL_CORE=2 \
	-DMGBA_STANDALONE \
	-DM_CORE_GBA \
	-DCORE_EXPORT

LINK_FLAGS = \
	/nodefaultlib \
	/nologo \
	/dll

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	kernel32.lib \
	shell32.lib \
	shlwapi.lib \
	ole32.lib \
	ws2_32.lib \

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

!IFDEF EMU
LIB_NAME = emu.lib
OBJS = $(EMU_OBJS)
!ELSE
LIB_NAME = base.lib
OBJS = $(BASE_OBJS)
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)

all: clean clear
	@nmake /nologo objs BASE=1
	@nmake /nologo objs EMU=1
	@nmake /nologo main-obj
	link /out:$(NAME).dll $(LINK_FLAGS) core.obj base.lib emu.lib $(LIBS)

main-obj: clean-obj core.obj

objs: clean-obj $(OBJS)
	lib /nologo /out:$(LIB_NAME) *.obj

clean-obj:
	@-del /q *.obj 2>nul

clean: clean-obj
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul

clear:
	@cls
