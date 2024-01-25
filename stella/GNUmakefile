UNAME_S = $(shell uname -s)
ARCH = $(shell uname -m)
NAME = stella

.SUFFIXES: .cxx

.cxx.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

O = o
include makefile-shared

OBJS := $(OBJS) \
	$(PROJ)/src/os/unix/FSNodePOSIX.$(O)

FLAGS = \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-implicit-fallthrough \
	-fPIC

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

DEFS := $(DEFS) \
	-DBSPF_UNIX

OBJS := $(OBJS) \
	$(PROJ)/src/os/unix/OSystemUNIX.$(O) \
	$(PROJ)/src/os/unix/SerialPortUNIX.$(O)

LD_FLAGS := $(LD_FLAGS) \
	-Wl,--no-undefined

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
MIN_VER = 10.15
else
MIN_VER = 13.0
FLAGS := $(FLAGS) -fembed-bitcode
endif

FLAGS := $(FLAGS) \
	-Wno-unused-private-field \
	-Wno-unused-const-variable \
	-Wno-reorder-ctor \
	-m$(TARGET)-version-min=$(MIN_VER) \
	-isysroot $(shell xcrun --sdk $(TARGET) --show-sdk-path) \
	-arch $(ARCH)

INCLUDES := $(INCLUDES) \
	-I$(PROJ)/src/os/macos

DEFS := $(DEFS) \
	-DBSPF_MACOS

OBJS := $(OBJS) \
	$(PROJ)/src/os/macos/OSystemMACOS.$(O) \
	$(PROJ)/src/os/macos/SerialPortMACOS.$(O)

LD_FLAGS := $(LD_FLAGS) \
	-arch $(ARCH)

LIBS := $(LIBS) \
	-lc++

endif

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++17

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
