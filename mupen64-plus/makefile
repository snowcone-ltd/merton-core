PROJ = mupen64plus-core
PROJ_RSP = mupen64plus-rsp-cxd4
PROJ_RDP = angrylion-rdp-plus
NAME = mupen64plus

.asm.obj:
	nasm -f win64 -d WIN64 -i $(PROJ)\src\asm_defines\ $< -o linkage_x64.obj

ASM_OBJ = \
	$(PROJ)/src/device/r4300/new_dynarec/x64/linkage_x64.obj

RSP_OBJS = \
	$(PROJ_RSP)/vu/add.obj \
	$(PROJ_RSP)/vu/divide.obj \
	$(PROJ_RSP)/vu/logical.obj \
	$(PROJ_RSP)/vu/multiply.obj \
	$(PROJ_RSP)/vu/select.obj \
	$(PROJ_RSP)/vu/vu.obj \
	$(PROJ_RSP)/su.obj

RDP_OBJS = \
	$(PROJ_RDP)/src/core/n64video.obj \
	$(PROJ_RDP)/src/core/parallel.obj \
	$(PROJ_RDP)/src/plugin/mupen64plus/msg.obj \
	$(PROJ_RDP)/src/plugin/mupen64plus/screen.obj

SUB_OBJS = \
	$(PROJ)/subprojects/md5/md5.obj \
	$(PROJ)/subprojects/minizip/ioapi.obj \
	$(PROJ)/subprojects/minizip/unzip.obj \
	$(PROJ)/subprojects/minizip/zip.obj

CORE_OBJS = \
	$(PROJ)/src/api/callbacks.obj \
	$(PROJ)/src/api/common.obj \
	$(PROJ)/src/api/config.obj \
	$(PROJ)/src/api/frontend.obj \
	$(PROJ)/src/backends/api/video_capture_backend.obj \
	$(PROJ)/src/backends/plugins_compat/audio_plugin_compat.obj \
	$(PROJ)/src/backends/plugins_compat/input_plugin_compat.obj \
	$(PROJ)/src/backends/clock_ctime_plus_delta.obj \
	$(PROJ)/src/backends/dummy_video_capture.obj \
	$(PROJ)/src/backends/file_storage.obj \
	$(PROJ)/src/device/cart/af_rtc.obj \
	$(PROJ)/src/device/cart/cart.obj \
	$(PROJ)/src/device/cart/cart_rom.obj \
	$(PROJ)/src/device/cart/eeprom.obj \
	$(PROJ)/src/device/cart/flashram.obj \
	$(PROJ)/src/device/cart/is_viewer.obj \
	$(PROJ)/src/device/cart/sram.obj \
	$(PROJ)/src/device/controllers/paks/biopak.obj \
	$(PROJ)/src/device/controllers/paks/mempak.obj \
	$(PROJ)/src/device/controllers/paks/rumblepak.obj \
	$(PROJ)/src/device/controllers/paks/transferpak.obj \
	$(PROJ)/src/device/controllers/game_controller.obj \
	$(PROJ)/src/device/controllers/vru_controller.obj \
	$(PROJ)/src/device/dd/dd_controller.obj \
	$(PROJ)/src/device/dd/disk.obj \
	$(PROJ)/src/device/gb/gb_cart.obj \
	$(PROJ)/src/device/gb/m64282fp.obj \
	$(PROJ)/src/device/gb/mbc3_rtc.obj \
	$(PROJ)/src/device/memory/memory.obj \
	$(PROJ)/src/device/pif/bootrom_hle.obj \
	$(PROJ)/src/device/pif/cic.obj \
	$(PROJ)/src/device/pif/n64_cic_nus_6105.obj \
	$(PROJ)/src/device/pif/pif.obj \
	$(PROJ)/src/device/r4300/new_dynarec/new_dynarec.obj \
	$(PROJ)/src/device/r4300/cached_interp.obj \
	$(PROJ)/src/device/r4300/cp0.obj \
	$(PROJ)/src/device/r4300/cp1.obj \
	$(PROJ)/src/device/r4300/cp2.obj \
	$(PROJ)/src/device/r4300/idec.obj \
	$(PROJ)/src/device/r4300/instr_counters.obj \
	$(PROJ)/src/device/r4300/interrupt.obj \
	$(PROJ)/src/device/r4300/pure_interp.obj \
	$(PROJ)/src/device/r4300/r4300_core.obj \
	$(PROJ)/src/device/r4300/tlb.obj \
	$(PROJ)/src/device/rcp/ai/ai_controller.obj \
	$(PROJ)/src/device/rcp/mi/mi_controller.obj \
	$(PROJ)/src/device/rcp/pi/pi_controller.obj \
	$(PROJ)/src/device/rcp/rdp/fb.obj \
	$(PROJ)/src/device/rcp/rdp/rdp_core.obj \
	$(PROJ)/src/device/rcp/ri/ri_controller.obj \
	$(PROJ)/src/device/rcp/rsp/rsp_core.obj \
	$(PROJ)/src/device/rcp/si/si_controller.obj \
	$(PROJ)/src/device/rcp/vi/vi_controller.obj \
	$(PROJ)/src/device/rdram/rdram.obj \
	$(PROJ)/src/device/device.obj \
	$(PROJ)/src/main/cheat.obj \
	$(PROJ)/src/main/eventloop.obj \
	$(PROJ)/src/main/main.obj \
	$(PROJ)/src/main/profile.obj \
	$(PROJ)/src/main/rom.obj \
	$(PROJ)/src/main/savestates.obj \
	$(PROJ)/src/main/sdl_key_converter.obj \
	$(PROJ)/src/main/util.obj \
	$(PROJ)/src/osal/files_win32.obj \
	$(PROJ)/src/plugin/dummy_audio.obj \
	$(PROJ)/src/plugin/dummy_input.obj \
	$(PROJ)/src/plugin/dummy_rsp.obj \
	$(PROJ)/src/plugin/dummy_video.obj \
	$(PROJ)/src/plugin/dummy_video.obj \
	$(PROJ)/src/plugin/plugin.obj

SHIM_OBJS = \
	shim/$(PROJ_RSP)/module.obj \
	shim/$(PROJ_RSP)/osal_dynamiclib_win32.obj \
	shim/$(PROJ_RDP)/gfx_m64p.obj \
	shim/$(PROJ_RDP)/vdac.obj \
	shim/$(PROJ)/SDL/SDL.obj \
	shim/$(PROJ)/zlib/adler32.obj \
	shim/$(PROJ)/zlib/crc32.obj \
	shim/$(PROJ)/zlib/deflate.obj \
	shim/$(PROJ)/zlib/gzclose.obj \
	shim/$(PROJ)/zlib/gzlib.obj \
	shim/$(PROJ)/zlib/gzread.obj \
	shim/$(PROJ)/zlib/gzwrite.obj \
	shim/$(PROJ)/zlib/inffast.obj \
	shim/$(PROJ)/zlib/inflate.obj \
	shim/$(PROJ)/zlib/inftrees.obj \
	shim/$(PROJ)/zlib/trees.obj \
	shim/$(PROJ)/zlib/zutil.obj \
	shim/$(PROJ)/screenshot.obj \
	shim/$(PROJ)/vidext.obj

OBJS = \
	$(ASM_OBJ) \
	$(RSP_OBJS) \
	$(RDP_OBJS) \
	$(SUB_OBJS) \
	$(CORE_OBJS) \
	$(SHIM_OBJS) \
	core.obj

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4057 \
	/wd4100 \
	/wd4101 \
	/wd4127 \
	/wd4131 \
	/wd4152 \
	/wd4189 \
	/wd4211 \
	/wd4244 \
	/wd4245 \
	/wd4267 \
	/wd4310 \
	/wd4324 \
	/wd4389 \
	/wd4456 \
	/wd4457 \
	/wd4459 \
	/wd4701 \
	/wd4702 \
	/wd4703 \
	/wd4706 \
	/wd4996 \
	/volatile:iso \
	/std:c11 \
	/nologo

INCLUDES = \
	-I../../libmatoya/src \
	-I$(PROJ_RDP)/src \
	-I$(PROJ)/src \
	-I$(PROJ)/src/api \
	-I$(PROJ)/subprojects \
	-I$(PROJ)/subprojects/md5 \
	-I$(PROJ)/subprojects/xxhash \
	-I$(PROJ)/subprojects/minizip \
	-Ishim/$(PROJ_RDP) \
	-Ishim/$(PROJ)/SDL \
	-Ishim/$(PROJ)/zlib

DEFS = \
	-DWIN32 \
	-D_CRT_SECURE_NO_WARNINGS \
	-DNEW_DYNAREC=NEW_DYNAREC_X64 \
	-DNO_KEYBINDINGS \
	-DM64P_PLUGIN_API \
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
	libcpmt.lib \
	kernel32.lib \
	oldnames.lib \
	ws2_32.lib \
	shlwapi.lib \
	shell32.lib \
	winmm.lib

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CPPFLAGS = $(CFLAGS) /EHsc

all: clean clear asm-defines $(OBJS)
	link /out:$(NAME).dll $(LINK_FLAGS) *.obj $(LIBS)

asm-defines:
	$(PROJ)\tools\gen_asm_script.cmd ..\..\src\asm_defines\ ..\..\..\ /D "__x86_64__" /D "NEW_DYNAREC=2"

clean:
	@-del /q *.obj 2>nul
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul

clear:
	@cls
