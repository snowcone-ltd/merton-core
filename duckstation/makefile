!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

OBJS = $(OBJS) \
	$(X64_OBJS) \
	$(PROJ)/src/common/fastjmp_x86.$(O)

FLAGS = $(FLAGS) \
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
	/wd5244

DEFS = $(DEFS) \
	-D_CRT_SECURE_NO_WARNINGS

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib \
	user32.lib \
	advapi32.lib \
	winmm.lib \
	onecore.lib # This is an umbrella lib, needed for memoryapi.h

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CPPFLAGS = $(CFLAGS) /EHsc /std:c++20
AFLAGS = /nologo /D _M_X86_64

all: clean clear fmt-interface $(OBJS)
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
