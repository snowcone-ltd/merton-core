!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

FLAGS = $(FLAGS) \
	/wd4005 /wd4018 /wd4057 \
	/wd4098 /wd4100 /wd4127 \
	/wd4132 /wd4133 /wd4146 \
	/wd4201 /wd4244 /wd4245 \
	/wd4267 /wd4273 /wd4295 \
	/wd4334 /wd4389 /wd4456 \
	/wd4457 /wd4701 /wd4702 \
	/wd4706 /wd4996

DEFS = $(DEFS) \
	-D_CRT_SECURE_NO_WARNINGS

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	kernel32.lib \
	shell32.lib \
	shlwapi.lib \
	ole32.lib \
	ws2_32.lib \
	oldnames.lib

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) /std:c11

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
