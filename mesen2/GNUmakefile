include ../unix-shared
include makefile-shared

FLAGS := $(FLAGS) \
	-Wno-ignored-qualifiers \
	-Wno-missing-field-initializers \
	-Wno-unused-variable \
	-Wno-unused-parameter \
	-Wno-sign-compare \
	-Wno-deprecated-declarations \
	-Wno-undefined-inline \
	-Wno-unused-private-field \
	-Wno-inconsistent-missing-override \
	-Wno-bitwise-instead-of-logical \
	-Wno-missing-braces

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

LIBS := $(LIBS) \
	-lc++

endif ### Apple

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++17 -Wno-deprecated-copy

all: clean clear
	make objs -j16

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

clean:
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
