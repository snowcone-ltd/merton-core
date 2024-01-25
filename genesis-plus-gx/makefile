!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

FLAGS = $(FLAGS) \
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
	/wd4706

DEFS = $(DEFS) \
	-DUNICODE \
	-D_CRT_SECURE_NO_WARNINGS

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
