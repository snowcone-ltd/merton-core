NAME = mupen64plus

O = obj
!INCLUDE makefile-shared

.asm.obj:
	deps\nasm -f win64 -d WIN64 -i $(PROJ)\src\asm_defines\ $< -o linkage_x64.obj

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

DEFS = $(DEFS) \
	-DWIN32 \
	-D_CRT_SECURE_NO_WARNINGS

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
	userenv.lib \
	user32.lib \
	ws2_32.lib \
	shlwapi.lib \
	shell32.lib \
	winmm.lib \
	advapi32.lib

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
