!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

.cpp.obj:
	clang $(INCLUDES) $(DEFS) $(FLAGS) -std=c++14 -c -o $@ $<

.c.obj:
	clang $(INCLUDES) $(DEFS) $(FLAGS) -std=c11 -c -o $@ $<

FLAGS = \
	-Wall \
	-Wextra \
	-fwrapv \
	-Wno-switch \
	-Wno-unused-parameter \
	-Wno-unknown-pragmas \
	-Wno-missing-field-initializers \
	-Wno-deprecated-literal-operator \
	-Wno-deprecated-declarations \
	-Wno-unused-function \
	-Wno-null-pointer-subtraction \
	-Wno-cast-function-type-mismatch \
	-Wno-unused-but-set-variable \
	-Wno-logical-not-parentheses \
	-Wno-sign-compare \
	-Wno-unused-variable \
	-Wno-self-assign \
	-Wno-shift-negative-value

DEFS = $(DEFS) \
	-DWIN32 \
	-D_CRT_SECURE_NO_WARNINGS \
	-DMDFN_DISABLE_NO_OPT_ERRWARN \
	-DMDFN_DISABLE_PICPIE_ERRWARN \
	-D__USING_SJLJ_EXCEPTIONS__ \
	-DHAVE_INLINEASM_AVX \
	-DSIZEOF_LONG=4 \
	-DARCH_X86_64 \
	-DMDFN_PSS_STYLE=2 \
	-Dstrdup=_strdup \
	-Dstrcmpi=_stricmp

INCLUDES = $(INCLUDES) \
	-IC:\Users\cdd\Documents\clang\include

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib \
	winmm.lib

L_FLAGS = \
	/nodefaultlib \
	/nologo \
	/dll

!IFDEF DEBUG
FLAGS = $(FLAGS) -O0 -g
L_FLAGS = $(L_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) -O2
!ENDIF

OBJS = $(OBJS) \
	$(PROJ)/src/mthreading/MThreading_Win32.$(O) \
	$(PROJ)/src/time/Time_Win32.$(O) \
	$(PROJ)/src/win32-common.$(O)

all: clear $(OBJS)
	link /out:$(NAME).dll $(L_FLAGS) $(OBJS) $(LIBS)

clean:
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul
	@-del /S *.obj

clear:
	@cls
