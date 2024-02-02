include ../unix-shared
include makefile-shared

OBJS := $(OBJS) \
	$(PROJ)/src/common/fastjmp.$(O)

FLAGS := $(FLAGS) \
	-Wno-typedef-redefinition \
	-Wno-missing-braces \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-Wno-sign-compare \
	-Wno-unused-variable \
	-Wno-unused-result \
	-Wno-unused-function \
	-Wno-implicit-fallthrough \
	-Wno-unused-but-set-variable \
	-Wno-enum-conversion \
	-Wno-switch

DEFS := $(DEFS) \
	-D_GNU_SOURCE

LIBS = \
	-lc

ifeq ($(ARCH), x86_64)
OBJS := $(OBJS) $(X64_OBJS)
else
INCLUDES := $(INCLUDES) $(ARM64_INCLUDES)
OBJS := $(OBJS) $(ARM64_OBJS)
DEFS := $(DEFS) $(ARM64_DEFS)
endif

### Linux
ifeq ($(UNAME_S), Linux)

LIBS := $(LIBS) \
	-lm \
	-lgcc_s \
	-lstdc++

endif ### Linux

### Apple
ifeq ($(UNAME_S), Darwin)

LIBS := $(LIBS) \
	-lc++

endif ### Apple

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) $(EXTRA_CXX_FLAGS) -std=c++20

all: clean clear
	make objs -j16

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

clean:
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
