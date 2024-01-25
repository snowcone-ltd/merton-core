include ../unix-shared
include makefile-shared

FLAGS := $(FLAGS) \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-sign-compare \
	-Wno-unused-result \
	-Wno-implicit-fallthrough \
	-Wno-unused-function

LIBS = \
	-lc \
	-lm

### Linux
ifeq ($(UNAME_S), Linux)
endif ### Linux

### Apple
ifeq ($(UNAME_S), Darwin)
endif ### Apple

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
