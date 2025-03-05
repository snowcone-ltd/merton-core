!INCLUDE ..\windows-shared
!INCLUDE makefile-shared

.cpp.obj:
	clang $(INCLUDES) $(DEFS) $(FLAGS) -c -o $@ $<

.c.obj:
	clang $(INCLUDES) $(DEFS) $(FLAGS) -c -o $@ $<

FLAGS = \
	-Wall \
	-Wextra \
	-O2 \
	-Wno-unused-parameter \
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
	-Wno-shift-negative-value \
	-fwrapv \
	-fno-fast-math \
	-fomit-frame-pointer \
	-fstrict-aliasing \
	-fsigned-char \
	-fjump-tables \
	-mfunction-return=keep \
	-mno-retpoline \
	-fno-pic \
	-fno-PIC \
	-fno-pie \
	-fno-PIE

DEFS = $(DEFS) \
	-DWIN32 \
	-DMDFN_DISABLE_NO_OPT_ERRWARN \
	-D__USING_SJLJ_EXCEPTIONS__ \
	-DARCH_X86_64 \
	-DMDFN_PSS_STYLE=2 \
	-D_CRT_SECURE_NO_WARNINGS \
	-DMDFN_DISABLE_PICPIE_ERRWARN \
	-Dstrdup=_strdup \
	-Dstrcmpi=_stricmp \
	-DCORE_EXPORT

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
	link /out:$(NAME).dll /nodefaultlib /nologo /dll /debug $(OBJS) $(LIBS)

clean:
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul
	@-del /S *.obj

clear:
	@cls
