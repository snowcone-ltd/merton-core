include ../unix-shared
include makefile-shared

FLAGS := $(FLAGS) \
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

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99

all: clean clear
	make objs -j16

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

clean:
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
