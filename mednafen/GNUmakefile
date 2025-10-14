include ../unix-shared
include makefile-shared

FLAGS := $(FLAGS) \
	-Wall \
	-Wextra \
	-fwrapv \
	-Wno-switch \
	-Wno-unused-parameter \
	-Wno-unknown-pragmas \
	-Wno-missing-field-initializers \
	-Wno-deprecated-declarations \
	-Wno-unused-function \
	-Wno-null-pointer-subtraction \
	-Wno-unused-but-set-variable \
	-Wno-logical-not-parentheses \
	-Wno-sign-compare \
	-Wno-unused-variable \
	-Wno-self-assign \
	-Wno-shift-negative-value \
	-Wno-deprecated-register \
	-Wno-bitwise-instead-of-logical \
	-Wno-absolute-value

DEFS := $(DEFS) \
	-DHAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE_NP \
	-D_FILE_OFFSET_BITS=64 \
	-DMDFN_PSS_STYLE=1 \
	-DSIZEOF_LONG=8 \
	-DSIZEOF_OFF_T=8

LIBS = \
	-lc

### Linux
ifeq ($(UNAME_S), Linux)

LIBS := $(LIBS) \
	-lm \
	-lgcc_s \
	-lstdc++

endif ### Linux

### Apple
ifeq ($(UNAME_S), Darwin)

DEFS := $(DEFS) \
	-DMDFN_DISABLE_PICPIE_ERRWARN

LIBS := $(LIBS) \
	-lc++

endif ### Apple

OBJS := $(OBJS) \
	$(PROJ)/src/mthreading/MThreading_POSIX.$(O) \
	$(PROJ)/src/time/Time_POSIX.$(O)

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++14

all: clean clear
	make objs -j16

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

clean:
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
