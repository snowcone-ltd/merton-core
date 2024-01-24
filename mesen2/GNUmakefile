UNAME_S = $(shell uname -s)
ARCH = $(shell uname -m)
NAME = mesen2

O = o
include makefile-shared

FLAGS = \
	-Wall \
	-Wextra \
	-Wno-ignored-qualifiers \
	-Wno-missing-field-initializers \
	-Wno-unused-variable \
	-Wno-unused-parameter \
	-Wno-sign-compare \
	-fPIC

DEFS := $(DEFS) \

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

FLAGS := $(FLAGS) \
	-Wno-misleading-indentation \
	-Wno-parentheses \
	-Wno-implicit-fallthrough \
	-Wno-type-limits \
	-Wno-switch

LIBS := $(LIBS) \
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
MIN_VER = 10.15
else
MIN_VER = 13.0
FLAGS := $(FLAGS) -fembed-bitcode
endif

FLAGS := $(FLAGS) \
	-Wno-deprecated-declarations \
	-Wno-undefined-inline \
	-Wno-unused-private-field \
	-Wno-inconsistent-missing-override \
	-Wno-bitwise-instead-of-logical \
	-m$(TARGET)-version-min=$(MIN_VER) \
	-isysroot $(shell xcrun --sdk $(TARGET) --show-sdk-path) \
	-arch $(ARCH)

LD_FLAGS := $(LD_FLAGS) \
	-arch $(ARCH)

LIBS := $(LIBS) \
	-lc++

endif

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++17 -Wno-deprecated-copy

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
