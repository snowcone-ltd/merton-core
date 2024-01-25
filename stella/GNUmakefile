include ../unix-shared
include makefile-shared

.SUFFIXES: .cxx

.cxx.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

OBJS := $(OBJS) \
	$(PROJ)/src/os/unix/FSNodePOSIX.$(O)

FLAGS := $(FLAGS) \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-implicit-fallthrough \
	-Wno-reorder

LIBS = \
	-lc

### Linux
ifeq ($(UNAME_S), Linux)

DEFS := $(DEFS) \
	-DBSPF_UNIX

OBJS := $(OBJS) \
	$(PROJ)/src/os/unix/OSystemUNIX.$(O) \
	$(PROJ)/src/os/unix/SerialPortUNIX.$(O)

LIBS := $(LIBS) \
	-lm \
	-lgcc_s \
	-lstdc++

endif ### Linux

### Apple
ifeq ($(UNAME_S), Darwin)

FLAGS := $(FLAGS) \
	-Wno-unused-private-field \
	-Wno-unused-const-variable \
	-Wno-reorder-ctor

INCLUDES := $(INCLUDES) \
	-I$(PROJ)/src/os/macos

DEFS := $(DEFS) \
	-DBSPF_MACOS

OBJS := $(OBJS) \
	$(PROJ)/src/os/macos/OSystemMACOS.$(O) \
	$(PROJ)/src/os/macos/SerialPortMACOS.$(O)

LIBS := $(LIBS) \
	-lc++

endif ### Apple

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
