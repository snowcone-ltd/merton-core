include ../unix-shared
include makefile-shared

FLAGS := $(FLAGS) \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-unused-result \
	-Wno-missing-field-initializers \
	-Wno-implicit-fallthrough \
	-Wno-switch

DEFS := $(DEFS) \
	-D_POSIX_C_SOURCE=200809L \
	-DHAVE_LOCALTIME_R

LIBS = \
	-lc

### Linux
ifeq ($(UNAME_S), Linux)

DEFS := $(DEFS) \
	-DHAVE_LOCALE

LIBS := $(LIBS) \
	-lm

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
