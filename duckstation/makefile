PROJ = duckstation
NAME = duckstation

UTIL_OBJS = \
	$(PROJ)/src/util/cd_image.obj \
	$(PROJ)/src/util/cd_image_bin.obj \
	$(PROJ)/src/util/cd_image_chd.obj \
	$(PROJ)/src/util/cd_image_cue.obj \
	$(PROJ)/src/util/cd_image_device.obj \
	$(PROJ)/src/util/cd_image_ecm.obj \
	$(PROJ)/src/util/cd_image_hasher.obj \
	$(PROJ)/src/util/cd_image_m3u.obj \
	$(PROJ)/src/util/cd_image_mds.obj \
	$(PROJ)/src/util/cd_image_memory.obj \
	$(PROJ)/src/util/cd_image_pbp.obj \
	$(PROJ)/src/util/cd_image_ppf.obj \
	$(PROJ)/src/util/cd_subchannel_replacement.obj \
	$(PROJ)/src/util/cd_xa.obj \
	$(PROJ)/src/util/cue_parser.obj \
	$(PROJ)/src/util/gpu_texture.obj \
	$(PROJ)/src/util/host.obj \
	$(PROJ)/src/util/ini_settings_interface.obj \
	$(PROJ)/src/util/iso_reader.obj \
	$(PROJ)/src/util/jit_code_buffer.obj \
	$(PROJ)/src/util/page_fault_handler.obj \
	$(PROJ)/src/util/platform_misc_win32.obj \
	$(PROJ)/src/util/postprocessing.obj \
	$(PROJ)/src/util/postprocessing_shader.obj \
	$(PROJ)/src/util/postprocessing_shader_fx.obj \
	$(PROJ)/src/util/postprocessing_shader_glsl.obj \
	$(PROJ)/src/util/shadergen.obj \
	$(PROJ)/src/util/shiftjis.obj \
	$(PROJ)/src/util/state_wrapper.obj \
	$(PROJ)/src/util/wav_writer.obj

DEPS_OBJS = \
	$(PROJ)/dep/libchdr/src/libchdr_bitstream.obj \
	$(PROJ)/dep/libchdr/src/libchdr_cdrom.obj \
	$(PROJ)/dep/libchdr/src/libchdr_chd.obj \
	$(PROJ)/dep/libchdr/src/libchdr_flac.obj \
	$(PROJ)/dep/libchdr/src/libchdr_huffman.obj \
	$(PROJ)/dep/lzma/src/Alloc.obj \
	$(PROJ)/dep/lzma/src/Bra86.obj \
	$(PROJ)/dep/lzma/src/BraIA64.obj \
	$(PROJ)/dep/lzma/src/CpuArch.obj \
	$(PROJ)/dep/lzma/src/Delta.obj \
	$(PROJ)/dep/lzma/src/LzFind.obj \
	$(PROJ)/dep/lzma/src/Lzma86Dec.obj \
	$(PROJ)/dep/lzma/src/Lzma86Enc.obj \
	$(PROJ)/dep/lzma/src/LzmaDec.obj \
	$(PROJ)/dep/lzma/src/LzmaEnc.obj \
	$(PROJ)/dep/lzma/src/LzmaLib.obj \
	$(PROJ)/dep/lzma/src/Sort.obj \
	$(PROJ)/dep/minizip/src/ioapi.obj \
	$(PROJ)/dep/minizip/src/iowin32.obj \
	$(PROJ)/dep/minizip/src/mztools.obj \
	$(PROJ)/dep/minizip/src/unzip.obj \
	$(PROJ)/dep/minizip/src/zip.obj \
	$(PROJ)/dep/reshadefx/src/effect_codegen_glsl.obj \
	$(PROJ)/dep/reshadefx/src/effect_codegen_hlsl.obj \
	$(PROJ)/dep/reshadefx/src/effect_expression.obj \
	$(PROJ)/dep/reshadefx/src/effect_lexer.obj \
	$(PROJ)/dep/reshadefx/src/effect_parser_exp.obj \
	$(PROJ)/dep/reshadefx/src/effect_parser_stmt.obj \
	$(PROJ)/dep/reshadefx/src/effect_symbol_table.obj \
	$(PROJ)/dep/stb/src/stb_image.obj \
	$(PROJ)/dep/stb/src/stb_image_resize.obj \
	$(PROJ)/dep/stb/src/stb_image_write.obj \
	$(PROJ)/dep/zlib/src/adler32.obj \
	$(PROJ)/dep/zlib/src/compress.obj \
	$(PROJ)/dep/zlib/src/crc32.obj \
	$(PROJ)/dep/zlib/src/deflate.obj \
	$(PROJ)/dep/zlib/src/gzclose.obj \
	$(PROJ)/dep/zlib/src/gzlib.obj \
	$(PROJ)/dep/zlib/src/gzread.obj \
	$(PROJ)/dep/zlib/src/gzwrite.obj \
	$(PROJ)/dep/zlib/src/infback.obj \
	$(PROJ)/dep/zlib/src/inffast.obj \
	$(PROJ)/dep/zlib/src/inflate.obj \
	$(PROJ)/dep/zlib/src/inftrees.obj \
	$(PROJ)/dep/zlib/src/trees.obj \
	$(PROJ)/dep/zlib/src/uncompr.obj \
	$(PROJ)/dep/zlib/src/zutil.obj \
	$(PROJ)/dep/zstd/lib/common/debug.obj \
	$(PROJ)/dep/zstd/lib/common/entropy_common.obj \
	$(PROJ)/dep/zstd/lib/common/error_private.obj \
	$(PROJ)/dep/zstd/lib/common/fse_decompress.obj \
	$(PROJ)/dep/zstd/lib/common/pool.obj \
	$(PROJ)/dep/zstd/lib/common/zstd_common.obj \
	$(PROJ)/dep/zstd/lib/compress/fse_compress.obj \
	$(PROJ)/dep/zstd/lib/compress/hist.obj \
	$(PROJ)/dep/zstd/lib/compress/huf_compress.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_compress.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_compress_literals.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_compress_sequences.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_compress_superblock.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_double_fast.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_fast.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_lazy.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_ldm.obj \
	$(PROJ)/dep/zstd/lib/compress/zstd_opt.obj \
	$(PROJ)/dep/zstd/lib/compress/zstdmt_compress.obj \
	$(PROJ)/dep/zstd/lib/decompress/huf_decompress.obj \
	$(PROJ)/dep/zstd/lib/decompress/zstd_ddict.obj \
	$(PROJ)/dep/zstd/lib/decompress/zstd_decompress.obj \
	$(PROJ)/dep/zstd/lib/decompress/zstd_decompress_block.obj \
	$(PROJ)/dep/xxhash/src/xxh_x86dispatch.obj \
	$(PROJ)/dep/xxhash/src/xxhash.obj

COMMON_OBJS = \
	$(PROJ)/src/common/thirdparty/SmallVector.obj \
	$(PROJ)/src/common/assert.obj \
	$(PROJ)/src/common/byte_stream.obj \
	$(PROJ)/src/common/error.obj \
	$(PROJ)/src/common/fastjmp_x86.obj \
	$(PROJ)/src/common/file_system.obj \
	$(PROJ)/src/common/image.obj \
	$(PROJ)/src/common/layered_settings_interface.obj \
	$(PROJ)/src/common/log.obj \
	$(PROJ)/src/common/md5_digest.obj \
	$(PROJ)/src/common/memmap.obj \
	$(PROJ)/src/common/progress_callback.obj \
	$(PROJ)/src/common/small_string.obj \
	$(PROJ)/src/common/string_util.obj \
	$(PROJ)/src/common/threading.obj \
	$(PROJ)/src/common/timer.obj

CORE_OBJS = \
	$(PROJ)/src/core/analog_controller.obj \
	$(PROJ)/src/core/analog_joystick.obj \
	$(PROJ)/src/core/bios.obj \
	$(PROJ)/src/core/bus.obj \
	$(PROJ)/src/core/cdrom.obj \
	$(PROJ)/src/core/cdrom_async_reader.obj \
	$(PROJ)/src/core/cheats.obj \
	$(PROJ)/src/core/controller.obj \
	$(PROJ)/src/core/cpu_code_cache.obj \
	$(PROJ)/src/core/cpu_core.obj \
	$(PROJ)/src/core/cpu_disasm.obj \
	$(PROJ)/src/core/cpu_newrec_compiler.obj \
	$(PROJ)/src/core/cpu_newrec_compiler_x64.obj \
	$(PROJ)/src/core/cpu_pgxp.obj \
	$(PROJ)/src/core/cpu_recompiler_code_generator.obj \
	$(PROJ)/src/core/cpu_recompiler_code_generator_generic.obj \
	$(PROJ)/src/core/cpu_recompiler_code_generator_x64.obj \
	$(PROJ)/src/core/cpu_recompiler_register_cache.obj \
	$(PROJ)/src/core/cpu_types.obj \
	$(PROJ)/src/core/digital_controller.obj \
	$(PROJ)/src/core/dma.obj \
	$(PROJ)/src/core/game_database.obj \
	$(PROJ)/src/core/gpu.obj \
	$(PROJ)/src/core/gpu_backend.obj \
	$(PROJ)/src/core/gpu_commands.obj \
	$(PROJ)/src/core/gpu_hw.obj \
	$(PROJ)/src/core/gpu_hw_shadergen.obj \
	$(PROJ)/src/core/gpu_shadergen.obj \
	$(PROJ)/src/core/gpu_sw.obj \
	$(PROJ)/src/core/gpu_sw_backend.obj \
	$(PROJ)/src/core/gte.obj \
	$(PROJ)/src/core/guncon.obj \
	$(PROJ)/src/core/host_interface_progress_callback.obj \
	$(PROJ)/src/core/interrupt_controller.obj \
	$(PROJ)/src/core/mdec.obj \
	$(PROJ)/src/core/memory_card.obj \
	$(PROJ)/src/core/memory_card_image.obj \
	$(PROJ)/src/core/multitap.obj \
	$(PROJ)/src/core/negcon.obj \
	$(PROJ)/src/core/pad.obj \
	$(PROJ)/src/core/pcdrv.obj \
	$(PROJ)/src/core/playstation_mouse.obj \
	$(PROJ)/src/core/psf_loader.obj \
	$(PROJ)/src/core/settings.obj \
	$(PROJ)/src/core/sio.obj \
	$(PROJ)/src/core/spu.obj \
	$(PROJ)/src/core/system.obj \
	$(PROJ)/src/core/texture_replacements.obj \
	$(PROJ)/src/core/timers.obj \
	$(PROJ)/src/core/timing_event.obj

SHIM_OBJS = \
	core.obj \
	shim/achievements.obj \
	shim/audio_stream.obj \
	shim/fullscreen_ui.obj \
	shim/game_list.obj \
	shim/gpu_device.obj \
	shim/imgui.obj \
	shim/imgui_manager.obj \
	shim/imgui_overlays.obj \
	shim/input_manager.obj \
	shim/nogui_host.obj \

OBJS = \
	rename/core_host.obj \
	rename/effect_preprocessor.obj \
	rename/format.obj \
	rename/os.obj \
	rename/zstd_threading.obj \
	rename/zstd_xxhash.obj \
	$(SHIM_OBJS) \
	$(DEPS_OBJS) \
	$(UTIL_OBJS) \
	$(COMMON_OBJS) \
	$(CORE_OBJS)

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4018 \
	/wd4057 \
	/wd4100 \
	/wd4127 \
	/wd4131 \
	/wd4189 \
	/wd4244 \
	/wd4245 \
	/wd4267 \
	/wd4305 \
	/wd4324 \
	/wd4334 \
	/wd4389 \
	/wd4456 \
	/wd4458 \
	/wd4505 \
	/wd4702 \
	/wd4706 \
	/wd4714 \
	/wd4996 \
	/wd5244 \
	/volatile:iso \
	/nologo

INCLUDES = \
	-I../../libmatoya/src \
	-Irename \
	-I$(PROJ)/src \
	-I$(PROJ)/dep \
	-I$(PROJ)/src/core \
	-I$(PROJ)/dep/fast_float/include \
	-I$(PROJ)/dep/fmt/include \
	-I$(PROJ)/dep/imgui/include \
	-I$(PROJ)/dep/libchdr/include \
	-I$(PROJ)/dep/lzma/include \
	-I$(PROJ)/dep/minizip/include \
	-I$(PROJ)/dep/rapidjson/include \
	-I$(PROJ)/dep/reshadefx/include \
	-I$(PROJ)/dep/simpleini/include \
	-I$(PROJ)/dep/stb/include \
	-I$(PROJ)/dep/xbyak/xbyak \
	-I$(PROJ)/dep/xxhash/include \
	-I$(PROJ)/dep/zlib/include \
	-I$(PROJ)/dep/zstd/lib

DEFS = \
	-D_CRT_SECURE_NO_WARNINGS \
	-DCORE_EXPORT \
	-DENABLE_RECOMPILER_SUPPORT \
	-DENABLE_NEWREC \
	-D_7ZIP_ST

LINK_FLAGS = \
	/nodefaultlib \
	/nologo \
	/dll

LIBS = \
	..\..\libmatoya\bin\windows\x64\matoya.lib \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	user32.lib \
	winmm.lib \
	ole32.lib \
	version.lib \
	advapi32.lib \
	kernel32.lib \
	oldnames.lib \
	onecore.lib # This is an umbrella lib, needed for memoryapi.h

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CPPFLAGS = $(CFLAGS) /EHsc /std:c++20
AFLAGS = /nologo /D _M_X86_64

all: clear $(SHIM_OBJS) $(CORE_OBJS)
	link /out:$(NAME).dll $(LINK_FLAGS) *.obj $(LIBS)

fmt-interface:
	cl $(CPPFLAGS) /interface /c $(PROJ)/dep/fmt/src/fmt.cc

clean:
	@-del /q *.obj 2>nul
	@-del /q *.ifc 2>nul
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul

clear:
	@cls
