!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

INCLUDES = $(INCLUDES) \
	-I$(PROJ)/src/os/windows

OBJS = $(OBJS) \
	$(PROJ)/src/os/windows/FSNodeWINDOWS.$(O) \
	$(PROJ)/src/os/windows/OSystemWINDOWS.$(O) \
	$(PROJ)/src/os/windows/SerialPortWINDOWS.$(O)

FLAGS = $(FLAGS) \
	/wd4100 /wd4244

DEFS = $(DEFS) \
	-DBSPF_WINDOWS

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib \
	advapi32.lib \
	shell32.lib

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) /EHsc /std:c++17

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
