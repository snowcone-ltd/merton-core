PROJ = Genesis-Plus-GX
NAME = genesis-plus-gx

OBJS = \
	$(PROJ)/core/cart_hw/svp/ssp16.obj \
	$(PROJ)/core/cart_hw/svp/svp.obj \
	$(PROJ)/core/cart_hw/areplay.obj \
	$(PROJ)/core/cart_hw/eeprom_93c.obj \
	$(PROJ)/core/cart_hw/eeprom_i2c.obj \
	$(PROJ)/core/cart_hw/eeprom_spi.obj \
	$(PROJ)/core/cart_hw/ggenie.obj \
	$(PROJ)/core/cart_hw/md_cart.obj \
	$(PROJ)/core/cart_hw/megasd.obj \
	$(PROJ)/core/cart_hw/sms_cart.obj \
	$(PROJ)/core/cart_hw/sram.obj \
	$(PROJ)/core/cd_hw/cd_cart.obj \
	$(PROJ)/core/cd_hw/cdc.obj \
	$(PROJ)/core/cd_hw/cdd.obj \
	$(PROJ)/core/cd_hw/gfx.obj \
	$(PROJ)/core/cd_hw/pcm.obj \
	$(PROJ)/core/cd_hw/scd.obj \
	$(PROJ)/core/input_hw/activator.obj \
	$(PROJ)/core/input_hw/gamepad.obj \
	$(PROJ)/core/input_hw/graphic_board.obj \
	$(PROJ)/core/input_hw/input.obj \
	$(PROJ)/core/input_hw/lightgun.obj \
	$(PROJ)/core/input_hw/mouse.obj \
	$(PROJ)/core/input_hw/paddle.obj \
	$(PROJ)/core/input_hw/sportspad.obj \
	$(PROJ)/core/input_hw/teamplayer.obj \
	$(PROJ)/core/input_hw/terebi_oekaki.obj \
	$(PROJ)/core/input_hw/xe_1ap.obj \
	$(PROJ)/core/m68k/m68kcpu.obj \
	$(PROJ)/core/m68k/s68kcpu.obj \
	$(PROJ)/core/ntsc/md_ntsc.obj \
	$(PROJ)/core/ntsc/sms_ntsc.obj \
	$(PROJ)/core/sound/blip_buf.obj \
	$(PROJ)/core/sound/eq.obj \
	$(PROJ)/core/sound/psg.obj \
	$(PROJ)/core/sound/sound.obj \
	$(PROJ)/core/sound/ym2413.obj \
	$(PROJ)/core/sound/ym2612.obj \
	$(PROJ)/core/sound/ym3438.obj \
	$(PROJ)/core/z80/z80.obj \
	$(PROJ)/core/genesis.obj \
	$(PROJ)/core/io_ctrl.obj \
	$(PROJ)/core/loadrom.obj \
	$(PROJ)/core/mem68k.obj \
	$(PROJ)/core/membnk.obj \
	$(PROJ)/core/memz80.obj \
	$(PROJ)/core/state.obj \
	$(PROJ)/core/system.obj \
	$(PROJ)/core/vdp_ctrl.obj \
	$(PROJ)/core/vdp_render.obj \
	core.obj

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4018 \
	/wd4100 \
	/wd4127 \
	/wd4146 \
	/wd4201 \
	/wd4244 \
	/wd4245 \
	/wd4267 \
	/wd4389 \
	/wd4459 \
	/wd4456 \
	/wd4457 \
	/wd4701 \
	/wd4706 \
	/volatile:iso \
	/nologo

INCLUDES = \
	-I. \
	-I.. \
	-I../../libmatoya/src \
	-I$(PROJ)/core \
	-I$(PROJ)/core/cart_hw \
	-I$(PROJ)/core/cart_hw\svp \
	-I$(PROJ)/core/cd_hw \
	-I$(PROJ)/core/input_hw \
	-I$(PROJ)/core/m68k \
	-I$(PROJ)/core/ntsc \
	-I$(PROJ)/core/sound \
	-I$(PROJ)/core/z80

DEFS = \
	-DUNICODE \
	-D_CRT_SECURE_NO_WARNINGS \
	-DLSB_FIRST \
	-DHAVE_YM3438_CORE \
	-DINLINE="static inline" \
	-DUSE_16BPP_RENDERING \
	-DMAXROMSIZE=33554432 \
	-DCORE_EXPORT

LINK_FLAGS = \
	/nodefaultlib \
	/nologo \
	/dll

LIBS = \
	../../libmatoya/bin/windows/x64/matoya.lib \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	kernel32.lib \
	ws2_32.lib \
	shlwapi.lib \
	bcrypt.lib \
	crypt32.lib

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)

all: clean clear $(OBJS)
	link /out:$(NAME).dll $(LINK_FLAGS) *.obj $(LIBS)

clean:
	@-del /q *.obj 2>nul
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul

clear:
	@cls
