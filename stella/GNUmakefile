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

OBJS := $(OBJS) \
	$(PROJ)/src/os/unix/OSystemUNIX.$(O) \
	$(PROJ)/src/os/unix/SerialPortUNIX.$(O)

DEFS := $(DEFS) \
	-DBSPF_UNIX

LIBS := $(LIBS) \
	-lm \
	-lgcc_s \
	-lstdc++

endif ### Linux

### Apple
ifeq ($(UNAME_S), Darwin)

INCLUDES := $(INCLUDES) \
	-I$(PROJ)/src/os/macos

OBJS := $(OBJS) \
	$(PROJ)/src/os/macos/OSystemMACOS.$(O) \
	$(PROJ)/src/os/macos/SerialPortMACOS.$(O)

FLAGS := $(FLAGS) \
	-Wno-unused-private-field \
	-Wno-unused-const-variable \
	-Wno-reorder-ctor

DEFS := $(DEFS) \
	-DBSPF_MACOS

LIBS := $(LIBS) \
	-lc++

endif ### Apple

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99
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
