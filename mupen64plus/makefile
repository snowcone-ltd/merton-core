!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

.asm.obj:
	deps\nasm -f win64 -d WIN64 -i $(PROJ)\src\asm_defines\ $< -o linkage_x64.obj

OBJS = $(OBJS) \
	$(ASM_OBJ_X64)

FLAGS = $(FLAGS) \
	/wd4057 /wd4100 /wd4101 \
	/wd4127 /wd4131 /wd4152 \
	/wd4189 /wd4211 /wd4244 \
	/wd4245 /wd4267 /wd4310 \
	/wd4324 /wd4389 /wd4456 \
	/wd4457 /wd4459 /wd4701 \
	/wd4702 /wd4703 /wd4706 \
	/wd4996 /wd4018 /wd4458 \
	/wd4201

DEFS = $(DEFS) \
	$(X64_DEFS) \
	-D_CRT_SECURE_NO_WARNINGS \
	-DVK_USE_PLATFORM_WIN32_KHR \
	-DWIN32

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) /std:c11
CPPFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) /EHsc

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
