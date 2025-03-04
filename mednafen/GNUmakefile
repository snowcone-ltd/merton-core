include ../unix-shared
include makefile-shared

FLAGS := $(FLAGS) \

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

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++17

all: clean clear
	make objs -j16

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

clean:
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear
