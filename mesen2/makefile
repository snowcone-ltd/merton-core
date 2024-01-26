!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

FLAGS = $(FLAGS) \
	/wd4100 /wd4127 /wd4189 \
	/wd4244 /wd4245 /wd4310 \
	/wd4456 /wd4457 /wd4701 \
	/wd4706 /wd4723

DEFS = $(DEFS) \
	-DUNICODE

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib \
	ole32.lib \
	oleaut32.lib \
	ws2_32.lib

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)

CPPFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) \
	-D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING \
	/std:c++17 \
	/EHsc

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
