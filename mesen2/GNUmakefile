UNAME_S = $(shell uname -s)
ARCH = $(shell uname -m)
NAME = mesen2

SZ_OBJS = \
	../SevenZip/7zAlloc.o \
	../SevenZip/7zArcIn.o \
	../SevenZip/7zBuf.o \
	../SevenZip/7zCrc.o \
	../SevenZip/7zCrcOpt.o \
	../SevenZip/7zDec.o \
	../SevenZip/7zFile.o \
	../SevenZip/7zMemBuffer.o \
	../SevenZip/7zStream.o \
	../SevenZip/Bcj2.o \
	../SevenZip/Bra.o \
	../SevenZip/Bra86.o \
	../SevenZip/BraIA64.o \
	../SevenZip/CpuArch.o \
	../SevenZip/Delta.o \
	../SevenZip/Lzma2Dec.o \
	../SevenZip/LzmaDec.o \
	../SevenZip/Ppmd7.o \
	../SevenZip/Ppmd7Dec.o

UTIL_OBJS = \
	../Utilities/Audio/blip_buf.o \
	../Utilities/Audio/HermiteResampler.o \
	../Utilities/Audio/stb_vorbis.o \
	../Utilities/Audio/StereoCombFilter.o \
	../Utilities/Audio/StereoDelayFilter.o \
	../Utilities/Audio/StereoPanningFilter.o \
	../Utilities/Audio/WavReader.o \
	../Utilities/HQX/hq2x.o \
	../Utilities/HQX/hq3x.o \
	../Utilities/HQX/hq4x.o \
	../Utilities/HQX/init.o \
	../Utilities/KreedSaiEagle/2xSai.o \
	../Utilities/KreedSaiEagle/Super2xSai.o \
	../Utilities/KreedSaiEagle/SuperEagle.o \
	../Utilities/NTSC/nes_ntsc.o \
	../Utilities/NTSC/sms_ntsc.o \
	../Utilities/NTSC/snes_ntsc.o \
	../Utilities/Patches/BpsPatcher.o \
	../Utilities/Patches/IpsPatcher.o \
	../Utilities/Patches/UpsPatcher.o \
	../Utilities/Scale2x/scale2x.o \
	../Utilities/Scale2x/scale3x.o \
	../Utilities/Scale2x/scalebit.o \
	../Utilities/xBRZ/xbrz.o \
	../Utilities/spng.o \
	../Utilities/ArchiveReader.o \
	../Utilities/AutoResetEvent.o \
	../Utilities/CRC32.o \
	../Utilities/HexUtilities.o \
	../Utilities/miniz.o \
	../Utilities/PNGHelper.o \
	../Utilities/Serializer.o \
	../Utilities/sha1.o \
	../Utilities/SimpleLock.o \
	../Utilities/Socket.o \
	../Utilities/SZReader.o \
	../Utilities/Timer.o \
	../Utilities/UPnPPortMapper.o \
	../Utilities/UTF8Util.o \
	../Utilities/VirtualFile.o \
	../Utilities/ZipReader.o \
	../Utilities/ZipWriter.o

CORE_SHARED_OBJS = \
	../Core/Netplay/GameClient.o \
	../Core/Netplay/GameClientConnection.o \
	../Core/Netplay/GameConnection.o \
	../Core/Netplay/GameServer.o \
	../Core/Netplay/GameServerConnection.o \
	../Core/Shared/Audio/PcmReader.o \
	../Core/Shared/Audio/SoundResampler.o \
	../Core/Shared/Audio/WaveRecorder.o \
	../Core/Shared/Utilities/emu2413.o \
	../Core/Shared/Video/BaseVideoFilter.o \
	../Core/Shared/Video/DebugHud.o \
	../Core/Shared/Video/DrawStringCommand.o \
	../Core/Shared/Video/RotateFilter.o \
	../Core/Shared/Video/ScaleFilter.o \
	../Core/Shared/Video/SystemHud.o \
	../Core/Shared/BaseControlDevice.o \
	../Core/Shared/BaseControlManager.o \
	../Core/Shared/CdReader.o \
	../Core/Shared/CheatManager.o \
	../Core/Shared/DebuggerRequest.o \
	../Core/Shared/EmulatorLock.o \
	../Core/Shared/EmuSettings.o \
	../Core/Shared/InputHud.o \
	../Core/Shared/KeyManager.o \
	../Core/Shared/MessageManager.o \
	../Core/Shared/NotificationManager.o \
	../Core/Shared/RewindData.o \
	../Core/Shared/RewindManager.o

CORE_NES_OBJS = \
	../Core/NES/APU/BaseExpansionAudio.o \
	../Core/NES/APU/DeltaModulationChannel.o \
	../Core/NES/APU/NesApu.o \
	../Core/NES/HdPacks/HdAudioDevice.o \
	../Core/NES/HdPacks/HdNesPack.o \
	../Core/NES/HdPacks/HdNesPpu.o \
	../Core/NES/HdPacks/HdPackBuilder.o \
	../Core/NES/HdPacks/HdPackLoader.o \
	../Core/NES/HdPacks/HdVideoFilter.o \
	../Core/NES/HdPacks/OggMixer.o \
	../Core/NES/HdPacks/OggReader.o \
	../Core/NES/Loaders/FdsLoader.o \
	../Core/NES/Loaders/iNesLoader.o \
	../Core/NES/Loaders/NsfLoader.o \
	../Core/NES/Loaders/RomLoader.o \
	../Core/NES/Loaders/StudyBoxLoader.o \
	../Core/NES/Loaders/UnifLoader.o \
	../Core/NES/Mappers/FDS/Fds.o \
	../Core/NES/Mappers/FDS/FdsAudio.o \
	../Core/NES/Mappers/FDS/FdsInputButtons.o \
	../Core/NES/Mappers/NSF/NsfMapper.o \
	../Core/NES/Mappers/VsSystem/VsControlManager.o \
	../Core/NES/BaseMapper.o \
	../Core/NES/BaseNesPpu.o \
	../Core/NES/BisqwitNtscFilter.o \
	../Core/NES/GameDatabase.o \
	../Core/NES/MapperFactory.o \
	../Core/NES/NesConsole.o \
	../Core/NES/NesControlManager.o \
	../Core/NES/NesCpu.o \
	../Core/NES/NesDefaultVideoFilter.o \
	../Core/NES/NesHeader.o \
	../Core/NES/NesMemoryManager.o \
	../Core/NES/NesNtscFilter.o \
	../Core/NES/NesPpu.o \
	../Core/NES/NesSoundMixer.o

CORE_SMS_OBJS = \
	../Core/SMS/Carts/SmsCart.o \
	../Core/SMS/SmsBiosMapper.o \
	../Core/SMS/SmsConsole.o \
	../Core/SMS/SmsControlManager.o \
	../Core/SMS/SmsCpu.o \
	../Core/SMS/SmsFmAudio.o \
	../Core/SMS/SmsMemoryManager.o \
	../Core/SMS/SmsNtscFilter.o \
	../Core/SMS/SmsPsg.o \
	../Core/SMS/SmsVdp.o

CORE_PCE_OBJS = \
	../Core/PCE/CdRom/PceAdpcm.o \
	../Core/PCE/CdRom/PceArcadeCard.o \
	../Core/PCE/CdRom/PceAudioFader.o \
	../Core/PCE/CdRom/PceCdAudioPlayer.o \
	../Core/PCE/CdRom/PceCdRom.o \
	../Core/PCE/CdRom/PceScsiBus.o \
	../Core/PCE/Input/PceTurboTap.o \
	../Core/PCE/PceConsole.o \
	../Core/PCE/PceControlManager.o \
	../Core/PCE/PceCpu.o \
	../Core/PCE/PceCpu.Instructions.o \
	../Core/PCE/PceMemoryManager.o \
	../Core/PCE/PceNtscFilter.o \
	../Core/PCE/PcePsg.o \
	../Core/PCE/PcePsgChannel.o \
	../Core/PCE/PceSf2RomMapper.o \
	../Core/PCE/PceTimer.o \
	../Core/PCE/PceVce.o \
	../Core/PCE/PceVdc.o \
	../Core/PCE/PceVpc.o

CORE_GAMEBOY_OBJS = \
	../Core/Gameboy/APU/GbApu.o \
	../Core/Gameboy/APU/GbNoiseChannel.o \
	../Core/Gameboy/APU/GbSquareChannel.o \
	../Core/Gameboy/APU/GbWaveChannel.o \
	../Core/Gameboy/Gameboy.o \
	../Core/Gameboy/GbControlManager.o \
	../Core/Gameboy/GbCpu.o \
	../Core/Gameboy/GbDefaultVideoFilter.o \
	../Core/Gameboy/GbDmaController.o \
	../Core/Gameboy/GbMemoryManager.o \
	../Core/Gameboy/GbPpu.o \
	../Core/Gameboy/GbTimer.o

CORE_SNES_OBJS = \
	../Core/SNES/Coprocessors/BSX/BsxCart.o \
	../Core/SNES/Coprocessors/BSX/BsxMemoryPack.o \
	../Core/SNES/Coprocessors/BSX/BsxSatellaview.o \
	../Core/SNES/Coprocessors/BSX/BsxStream.o \
	../Core/SNES/Coprocessors/CX4/Cx4.o \
	../Core/SNES/Coprocessors/CX4/Cx4.Instructions.o \
	../Core/SNES/Coprocessors/DSP/NecDsp.o \
	../Core/SNES/Coprocessors/GSU/Gsu.o \
	../Core/SNES/Coprocessors/GSU/Gsu.Instructions.o \
	../Core/SNES/Coprocessors/MSU1/Msu1.o \
	../Core/SNES/Coprocessors/OBC1/Obc1.o \
	../Core/SNES/Coprocessors/SA1/Sa1.o \
	../Core/SNES/Coprocessors/SA1/Sa1Cpu.o \
	../Core/SNES/Coprocessors/SDD1/Sdd1.o \
	../Core/SNES/Coprocessors/SDD1/Sdd1Decomp.o \
	../Core/SNES/Coprocessors/SDD1/Sdd1Mmc.o \
	../Core/SNES/Coprocessors/SGB/SuperGameboy.o \
	../Core/SNES/Coprocessors/SPC7110/Rtc4513.o \
	../Core/SNES/Coprocessors/SPC7110/Spc7110.o \
	../Core/SNES/Coprocessors/SPC7110/Spc7110Decomp.o \
	../Core/SNES/DSP/Dsp.o \
	../Core/SNES/DSP/DspVoice.o \
	../Core/SNES/Input/Multitap.o \
	../Core/SNES/Input/SnesController.o \
	../Core/SNES/AluMulDiv.o \
	../Core/SNES/BaseCartridge.o \
	../Core/SNES/InternalRegisters.o \
	../Core/SNES/MemoryMappings.o \
	../Core/SNES/RegisterHandlerB.o \
	../Core/SNES/SnesConsole.o \
	../Core/SNES/SnesControlManager.o \
	../Core/SNES/SnesCpu.o \
	../Core/SNES/SnesDefaultVideoFilter.o \
	../Core/SNES/SnesDmaController.o \
	../Core/SNES/SnesMemoryManager.o \
	../Core/SNES/SnesNtscFilter.o \
	../Core/SNES/SnesPpu.o \
	../Core/SNES/Spc.o \
	../Core/SNES/Spc.Instructions.o

SHIM_OBJS = \
	Shim/Audio/SoundMixer.o \
	Shim/Debugger/Debugger.o \
	Shim/Debugger/PpuTools.o \
	Shim/Debugger/ScriptManager.o \
	Shim/Movies/MovieManager.o \
	Shim/Movies/MovieRecorder.o \
	Shim/SNES/Debugger/SnesPpuTools.o \
	Shim/Utilities/FolderUtilities.o \
	Shim/Video/VideoDecoder.o \
	Shim/Video/VideoRenderer.o \
	Shim/BatteryManager.o \
	Shim/Emulator.o \
	Shim/SaveStateManager.o \
	Shim/ShimKeyManager.o

OBJS = \
	$(SZ_OBJS) \
	$(UTIL_OBJS) \
	$(CORE_SHARED_OBJS) \
	$(CORE_NES_OBJS) \
	$(CORE_SMS_OBJS) \
	$(CORE_PCE_OBJS) \
	$(CORE_GAMEBOY_OBJS) \
	$(CORE_SNES_OBJS) \
	$(SHIM_OBJS) \
	core.o

INCLUDES = \
	-I.. \
	-I../Core \
	-I../Utilities

FLAGS = \
	-Wall \
	-Wextra \
	-Wno-ignored-qualifiers \
	-Wno-missing-field-initializers \
	-Wno-unused-variable \
	-Wno-unused-parameter \
	-Wno-sign-compare \
	-fPIC

DEFS = \
	-DCORE_EXPORT

LD_FLAGS = \
	-shared \
	-nodefaultlibs

LIBS = \
	-lc

ifdef DEBUG
FLAGS := $(FLAGS) -O0 -g3
else
FLAGS := $(FLAGS) -O3 -g0 -flto -fvisibility=hidden
LD_FLAGS := $(LD_FLAGS) -flto=auto
endif

#############
### LINUX ###
#############
ifeq ($(UNAME_S), Linux)

TARGET = linux
SUFFIX = so

FLAGS := $(FLAGS) \
	-Wno-misleading-indentation \
	-Wno-parentheses \
	-Wno-implicit-fallthrough \
	-Wno-type-limits \
	-Wno-switch

LIBS := $(LIBS) \
	-lstdc++

endif

#############
### APPLE ###
#############
ifeq ($(UNAME_S), Darwin)

ifndef TARGET
TARGET = macosx
endif

ifndef ARCH
ARCH = x86_64
endif

SUFFIX = dylib

ifeq ($(TARGET), macosx)
MIN_VER = 10.15
else
MIN_VER = 13.0
FLAGS := $(FLAGS) -fembed-bitcode
endif

FLAGS := $(FLAGS) \
	-Wno-deprecated-declarations \
	-Wno-undefined-inline \
	-Wno-unused-private-field \
	-Wno-inconsistent-missing-override \
	-Wno-bitwise-instead-of-logical \
	-m$(TARGET)-version-min=$(MIN_VER) \
	-isysroot $(shell xcrun --sdk $(TARGET) --show-sdk-path) \
	-arch $(ARCH)

LD_FLAGS := $(LD_FLAGS) \
	-arch $(ARCH)

LIBS := $(LIBS) \
	-lc++

endif

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c99
CXXFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) -std=c++17 -Wno-deprecated-copy

all: clean clear
	make objs -j4

objs: $(OBJS)
	$(CC) -o $(NAME).$(SUFFIX) $(LD_FLAGS) $(OBJS) $(LIBS)

clean:
	@rm -rf obj
	@rm -rf libs
	@rm -rf $(NAME).*
	@rm -rf $(OBJS)

clear:
	@clear

