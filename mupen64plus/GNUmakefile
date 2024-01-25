include ../unix-shared
include makefile-shared

.SUFFIXES: .asm

.asm.o:
	nasm -f elf64 -i $(PROJ)/src/asm_defines/ $< -o $@

FLAGS := $(FLAGS) \
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
	-Wno-switch

DEFS := $(DEFS) \
	-D_GNU_SOURCE

LIBS = \
	-lc \
	-lm

### Linux
ifeq ($(UNAME_S), Linux)

OBJS := $(OBJS) \
	$(ASM_OBJ_X64)

DEFS := $(DEFS) \
	-DNEW_DYNAREC=NEW_DYNAREC_X64

LIBS := $(LIBS) \
	-lgcc_s \
	-lstdc++

endif ### Linux

### Apple
ifeq ($(UNAME_S), Darwin)

ifeq ($(ARCH), x86_64)
OBJS := $(OBJS) $(ASM_OBJ_X64)
DEFS := $(DEFS) -DNEW_DYNAREC=NEW_DYNAREC_X64
else
OBJS := $(OBJS) $(ASM_OBJ_ARM64) # FIXME This does not compile! Need fix from M64P team
DEFS := $(DEFS) -DNEW_DYNAREC=NEW_DYNAREC_ARM64
endif

FLAGS := $(FLAGS) \
	-Wno-xor-used-as-pow \
	-Wno-tautological-overlap-compare \
	-Wno-null-pointer-subtraction

LIBS := $(LIBS) \
	-lc++

endif ### Apple

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

objs: $(OBJS)
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
