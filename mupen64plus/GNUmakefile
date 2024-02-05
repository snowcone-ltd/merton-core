include ../unix-shared
include makefile-shared

.SUFFIXES: .asm

.asm.o:
	nasm -f $(NASM_FORMAT) $(NASM_DEFS) -i $(PROJ)/src/asm_defines/ $< -o $@

OBJS := $(OBJS) \
	$(PRSP_OBJS)

FLAGS := $(FLAGS) \
	-Wno-unused-result \
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
	-Wno-xor-used-as-pow \
	-Wno-tautological-overlap-compare \
	-Wno-null-pointer-subtraction \
	-Wno-pointer-sign \
	-Wno-switch

DEFS := $(DEFS) \
	-D_GNU_SOURCE

LIBS = \
	-lc

ifeq ($(ARCH), x86_64)
OBJS := $(OBJS) $(ASM_OBJ_X64)
DEFS := $(DEFS) $(X64_DEFS)
else
# FIXME No dynarec support on arm64
# OBJS := $(OBJS) $(ASM_OBJ_ARM64)
DEFS := $(DEFS) $(ARM64_DEFS)
endif

### Linux
ifeq ($(UNAME_S), Linux)

NASM_FORMAT = elf64

LIBS := $(LIBS) \
	-lm \
	-lgcc_s \
	-lstdc++

endif ### Linux

### Apple
ifeq ($(UNAME_S), Darwin)

NASM_FORMAT = macho64
NASM_DEFS = -d LEADING_UNDERSCORE

LIBS := $(LIBS) \
	-lc++

endif ### Apple

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) \
	-Wno-incompatible-pointer-types \
	-Wno-absolute-value \
	-std=c99

CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++17
ASFLAGS = -I$(PROJ)/src/asm_defines

all: clean clear asm-defines
	make objs -j16

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

asm-obj: $(ASMD_OBJ)

asm-defines:
	make asm-obj NO_LTO=1
	dos2unix $(PROJ)/tools/gen_asm_script.sh
	bash $(PROJ)/tools/gen_asm_script.sh $(PROJ)/src/asm_defines $(ASMD_OBJ)

clean:
	@sed -i'' -e 's/__arm__/__aarch64__/g' $(PROJ_RSP)/my_types.h # ARM fix
	@rm -rf $(PROJ_RSP)/sse2neon # Use updates SSE2NEON.h
	@rm -rf $(PROJ)/src/asm_defines/*.h
	@rm -rf $(NAME).*
	@rm -rf $(OBJS) $(ASMD_OBJ)

clear:
	@clear
