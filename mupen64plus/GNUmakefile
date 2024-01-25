UNAME_S = $(shell uname -s)
ARCH = $(shell uname -m)
NAME = mupen64plus

O = o
include makefile-shared

.SUFFIXES: .asm

.asm.o:
	nasm -f elf64 -i $(PROJ)/src/asm_defines/ $< -o $@

FLAGS = \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-unused-but-set-parameter \
	-Wno-unused-but-set-variable \
	-Wno-implicit-fallthrough \
	-Wno-sign-compare \
	-Wno-unused-function \
	-Wno-unused-variable \
	-Wno-parentheses \
	-Wno-missing-field-initializers \
	-Wno-empty-body \
	-Wno-switch \
	-fPIC

DEFS := $(DEFS) \
	-D_GNU_SOURCE

LD_FLAGS = \
	-shared \
	-nodefaultlibs

LIBS = \
	-lc \
	-lm

ifdef DEBUG
FLAGS := $(FLAGS) -O0 -g3
else

FLAGS := $(FLAGS) -O3 -g0 -fvisibility=hidden
LD_FLAGS := $(LD_FLAGS)

ifndef NO_LTO
FLAGS := $(FLAGS) -flto
LD_FLAGS := $(LD_FLAGS) -flto=auto
endif

endif

#############
### LINUX ###
#############
ifeq ($(UNAME_S), Linux)

TARGET = linux
SUFFIX = so

ASM_OBJ = $(ASM_OBJ_X64)

FLAGS := $(FLAGS) \
	-DNEW_DYNAREC=NEW_DYNAREC_X64

LD_FLAGS := $(LD_FLAGS) \
	-Wl,--no-undefined

LIBS := $(LIBS) \
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

ifeq ($(ARCH), x86_64)
ASM_OBJ = $(ASM_OBJ_X64)
FLAGS := $(FLAGS) -DNEW_DYNAREC=NEW_DYNAREC_X64
else
ASM_OBJ = $(ASM_OBJ_ARM64) # FIXME This does not compile! Need fix from M64P team
FLAGS := $(FLAGS) -DNEW_DYNAREC=NEW_DYNAREC_ARM64
endif

SUFFIX = dylib

ifeq ($(TARGET), macosx)
MIN_VER = 10.15
else
MIN_VER = 13.0
FLAGS := $(FLAGS) -fembed-bitcode
endif

FLAGS := $(FLAGS) \
	-Wno-xor-used-as-pow \
	-Wno-tautological-overlap-compare \
	-Wno-null-pointer-subtraction \
	-m$(TARGET)-version-min=$(MIN_VER) \
	-isysroot $(shell xcrun --sdk $(TARGET) --show-sdk-path) \
	-arch $(ARCH)

LD_FLAGS := $(LD_FLAGS) \
	-arch $(ARCH)

LIBS := $(LIBS) \
	-lc++

endif

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) \
	-Wno-incompatible-pointer-types \
	-Wno-absolute-value \
	-std=c99

CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++17
ASFLAGS = -I$(PROJ)/src/asm_defines

LINK_OBJS = $(OBJS) \
	../../libmatoya/bin/$(TARGET)/$(ARCH)/libmatoya.a

all: clean clear asm-defines
	make objs -j8

objs: $(OBJS) $(ASM_OBJ)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(LINK_OBJS) $(LIBS)

asm-obj: $(ASMD_OBJ)

asm-defines:
	make asm-obj NO_LTO=1
	dos2unix $(PROJ)/tools/gen_asm_script.sh
	bash $(PROJ)/tools/gen_asm_script.sh $(PROJ)/src/asm_defines $(ASMD_OBJ)

clean:
	@rm -rf obj $(ASMD_OBJ)
	@rm -rf libs
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
