UNAME_S = $(shell uname -s)
ARCH = $(shell uname -m)
NAME = duckstation

O = o
include makefile-shared

OBJS := $(OBJS) \
	$(PROJ)/src/common/fastjmp.$(O)

FLAGS = \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-Wno-sign-compare \
	-Wno-unused-variable \
	-Wno-unused-result \
	-Wno-unused-function \
	-Wno-implicit-fallthrough \
	-Wno-unused-but-set-variable \
	-Wno-enum-conversion \
	-Wno-switch \
	-fPIC

DEFS := $(DEFS) \
	-D_GNU_SOURCE

LD_FLAGS = \
	-shared \
	-nodefaultlibs

LIBS = \
	-lc

ifdef DEBUG
FLAGS := $(FLAGS) -O0 -g3
else
FLAGS := $(FLAGS) -O3 -g0 -flto -fvisibility=hidden
LD_FLAGS := $(LD_FLAGS) -flto=auto
endif

#############
### LINUX ###
#############
ifeq ($(UNAME_S), Linux)

TARGET = linux
SUFFIX = so

OBJS := $(OBJS) \
	$(X64_OBJS)

FLAGS := $(FLAGS) \
	-Wno-maybe-uninitialized

LD_FLAGS := $(LD_FLAGS) \
	-Wl,--no-undefined

EXTRA_CXX_FLAGS = \
	-Wno-class-memaccess

LIBS := $(LIBS) \
	-lm \
	-lgcc_s \
	-lstdc++

endif

#############
### APPLE ###
#############
ifeq ($(UNAME_S), Darwin)

ifndef TARGET
TARGET = macosx
endif

ifndef ARCH
ARCH = x86_64
endif

SUFFIX = dylib

ifeq ($(TARGET), macosx)
MIN_VER = 11.0
else
MIN_VER = 13.0
FLAGS := $(FLAGS) -fembed-bitcode
endif

ifeq ($(ARCH), x86_64)
OBJS := $(OBJS) $(X64_OBJS)
else
OBJS := $(OBJS) $(ARM64_OBJS)
DEFS := $(DEFS) $(ARM64_DEFS)
INCLUDES := $(INCLUDES) $(ARM64_INCLUDES)
endif

FLAGS := $(FLAGS) \
	-Wno-typedef-redefinition \
	-Wno-missing-braces \
	-m$(TARGET)-version-min=$(MIN_VER) \
	-isysroot $(shell xcrun --sdk $(TARGET) --show-sdk-path) \
	-arch $(ARCH)

LD_FLAGS := $(LD_FLAGS) \
	-arch $(ARCH)

LIBS := $(LIBS) \
	-lc++

endif

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) $(EXTRA_CXX_FLAGS) -std=c++20

all: clean clear
	make objs -j8

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

clean:
	@rm -rf obj
	@rm -rf libs
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
