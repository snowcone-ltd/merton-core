UNAME_S = $(shell uname -s)
ARCH = $(shell uname -m)
NAME = genesis-plus-gx

O = o
include makefile-shared

FLAGS = \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-sign-compare \
	-Wno-unused-result \
	-Wno-implicit-fallthrough \
	-Wno-unused-function \
	-fPIC

DEFS := $(DEFS) \

LD_FLAGS = \
	-shared \
	-nodefaultlibs

LIBS = \
	-lc \
	-lm

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

FLAGS := $(FLAGS) \

LD_FLAGS := $(LD_FLAGS) \
	-Wl,--no-undefined

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
MIN_VER = 10.15
else
MIN_VER = 13.0
FLAGS := $(FLAGS) -fembed-bitcode
endif

FLAGS := $(FLAGS) \
	-m$(TARGET)-version-min=$(MIN_VER) \
	-isysroot $(shell xcrun --sdk $(TARGET) --show-sdk-path) \
	-arch $(ARCH)

LD_FLAGS := $(LD_FLAGS) \
	-arch $(ARCH)

endif

LINK_OBJS = \
	$(OBJS) \
	../../libmatoya/bin/$(TARGET)/$(ARCH)/libmatoya.a

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99

all: clean clear
	make objs -j8

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(LINK_OBJS) $(LIBS)

clean:
	@rm -rf obj
	@rm -rf libs
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
