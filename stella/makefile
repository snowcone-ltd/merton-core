NAME = stella

O = obj
!INCLUDE makefile-shared

INCLUDES = $(INCLUDES) \
	-I$(PROJ)/src/os/windows

OBJS = $(OBJS) \
	$(PROJ)/src/os/windows/FSNodeWINDOWS.$(O) \
	$(PROJ)/src/os/windows/OSystemWINDOWS.$(O) \
	$(PROJ)/src/os/windows/SerialPortWINDOWS.$(O)

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4100 \
	/wd4244 \
	/volatile:iso \
	/nologo

DEFS = $(DEFS) \
	-DBSPF_WINDOWS

LINK_FLAGS = \
	/nodefaultlib \
	/nologo \
	/dll

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib \
	advapi32.lib \
	shell32.lib

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CXXFLAGS = $(CFLAGS) /EHsc /std:c++17

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
