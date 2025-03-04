!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

.cpp.obj:
	clang $(INCLUDES) $(DEFS) $(FLAGS) -c -o $@ $<

.c.obj:
	clang $(INCLUDES) $(DEFS) $(FLAGS) -c -o $@ $<

FLAGS = \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-Wno-unknown-pragmas \
	-Wno-deprecated-literal-operator \
	-Wno-deprecated-declarations \
	-Wno-unused-function \
	-mavx

DEFS = $(DEFS) \
	-DMDFN_PSS_STYLE=2 \
	-D_CRT_SECURE_NO_WARNINGS \
	-DMDFN_DISABLE_PICPIE_ERRWARN \
	-Dstrdup=_strdup \
	-Dstrcmpi=_stricmp

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib \
	winmm.lib

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CPPFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)

all: clean clear $(OBJS)
	link /out:$(NAME).dll $(LINK_FLAGS) $(OBJS) $(LIBS)

clean:
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul
	@-del /S *.obj

clear:
	@cls
