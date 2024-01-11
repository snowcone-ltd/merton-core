PROJ = Mesen2
NAME = mesen2

SZ_OBJS = \
	$(PROJ)/SevenZip/7zAlloc.obj \
	$(PROJ)/SevenZip/7zArcIn.obj \
	$(PROJ)/SevenZip/7zBuf.obj \
	$(PROJ)/SevenZip/7zCrc.obj \
	$(PROJ)/SevenZip/7zCrcOpt.obj \
	$(PROJ)/SevenZip/7zDec.obj \
	$(PROJ)/SevenZip/7zFile.obj \
	$(PROJ)/SevenZip/7zMemBuffer.obj \
	$(PROJ)/SevenZip/7zStream.obj \
	$(PROJ)/SevenZip/Bcj2.obj \
	$(PROJ)/SevenZip/Bra.obj \
	$(PROJ)/SevenZip/Bra86.obj \
	$(PROJ)/SevenZip/BraIA64.obj \
	$(PROJ)/SevenZip/CpuArch.obj \
	$(PROJ)/SevenZip/Delta.obj \
	$(PROJ)/SevenZip/Lzma2Dec.obj \
	$(PROJ)/SevenZip/LzmaDec.obj \
	$(PROJ)/SevenZip/Ppmd7.obj \
	$(PROJ)/SevenZip/Ppmd7Dec.obj

UTIL_OBJS = \
	$(PROJ)/Utilities/Audio/blip_buf.obj \
	$(PROJ)/Utilities/Audio/HermiteResampler.obj \
	$(PROJ)/Utilities/Audio/stb_vorbis.obj \
	$(PROJ)/Utilities/Audio/StereoCombFilter.obj \
	$(PROJ)/Utilities/Audio/StereoDelayFilter.obj \
	$(PROJ)/Utilities/Audio/StereoPanningFilter.obj \
	$(PROJ)/Utilities/Audio/WavReader.obj \
	$(PROJ)/Utilities/HQX/hq2x.obj \
	$(PROJ)/Utilities/HQX/hq3x.obj \
	$(PROJ)/Utilities/HQX/hq4x.obj \
	$(PROJ)/Utilities/HQX/init.obj \
	$(PROJ)/Utilities/KreedSaiEagle/2xSai.obj \
	$(PROJ)/Utilities/KreedSaiEagle/Super2xSai.obj \
	$(PROJ)/Utilities/KreedSaiEagle/SuperEagle.obj \
	$(PROJ)/Utilities/NTSC/nes_ntsc.obj \
	$(PROJ)/Utilities/NTSC/sms_ntsc.obj \
	$(PROJ)/Utilities/NTSC/snes_ntsc.obj \
	$(PROJ)/Utilities/Patches/BpsPatcher.obj \
	$(PROJ)/Utilities/Patches/IpsPatcher.obj \
	$(PROJ)/Utilities/Patches/UpsPatcher.obj \
	$(PROJ)/Utilities/Scale2x/scale2x.obj \
	$(PROJ)/Utilities/Scale2x/scale3x.obj \
	$(PROJ)/Utilities/Scale2x/scalebit.obj \
	$(PROJ)/Utilities/xBRZ/xbrz.obj \
	$(PROJ)/Utilities/spng.obj \
	$(PROJ)/Utilities/ArchiveReader.obj \
	$(PROJ)/Utilities/AutoResetEvent.obj \
	$(PROJ)/Utilities/CRC32.obj \
	$(PROJ)/Utilities/HexUtilities.obj \
	$(PROJ)/Utilities/miniz.obj \
	$(PROJ)/Utilities/PNGHelper.obj \
	$(PROJ)/Utilities/Serializer.obj \
	$(PROJ)/Utilities/sha1.obj \
	$(PROJ)/Utilities/SimpleLock.obj \
	$(PROJ)/Utilities/Socket.obj \
	$(PROJ)/Utilities/SZReader.obj \
	$(PROJ)/Utilities/Timer.obj \
	$(PROJ)/Utilities/UPnPPortMapper.obj \
	$(PROJ)/Utilities/UTF8Util.obj \
	$(PROJ)/Utilities/VirtualFile.obj \
	$(PROJ)/Utilities/ZipReader.obj \
	$(PROJ)/Utilities/ZipWriter.obj

CORE_SHARED_OBJS = \
	$(PROJ)/Core/Netplay/GameClient.obj \
	$(PROJ)/Core/Netplay/GameClientConnection.obj \
	$(PROJ)/Core/Netplay/GameConnection.obj \
	$(PROJ)/Core/Netplay/GameServer.obj \
	$(PROJ)/Core/Netplay/GameServerConnection.obj \
	$(PROJ)/Core/Shared/Audio/PcmReader.obj \
	$(PROJ)/Core/Shared/Audio/SoundResampler.obj \
	$(PROJ)/Core/Shared/Audio/WaveRecorder.obj \
	$(PROJ)/Core/Shared/Utilities/emu2413.obj \
	$(PROJ)/Core/Shared/Video/BaseVideoFilter.obj \
	$(PROJ)/Core/Shared/Video/DebugHud.obj \
	$(PROJ)/Core/Shared/Video/DrawStringCommand.obj \
	$(PROJ)/Core/Shared/Video/RotateFilter.obj \
	$(PROJ)/Core/Shared/Video/ScaleFilter.obj \
	$(PROJ)/Core/Shared/Video/SystemHud.obj \
	$(PROJ)/Core/Shared/BaseControlDevice.obj \
	$(PROJ)/Core/Shared/BaseControlManager.obj \
	$(PROJ)/Core/Shared/CdReader.obj \
	$(PROJ)/Core/Shared/CheatManager.obj \
	$(PROJ)/Core/Shared/DebuggerRequest.obj \
	$(PROJ)/Core/Shared/EmulatorLock.obj \
	$(PROJ)/Core/Shared/EmuSettings.obj \
	$(PROJ)/Core/Shared/InputHud.obj \
	$(PROJ)/Core/Shared/KeyManager.obj \
	$(PROJ)/Core/Shared/MessageManager.obj \
	$(PROJ)/Core/Shared/NotificationManager.obj \
	$(PROJ)/Core/Shared/RewindData.obj \
	$(PROJ)/Core/Shared/RewindManager.obj

CORE_NES_OBJS = \
	$(PROJ)/Core/NES/APU/BaseExpansionAudio.obj \
	$(PROJ)/Core/NES/APU/DeltaModulationChannel.obj \
	$(PROJ)/Core/NES/APU/NesApu.obj \
	$(PROJ)/Core/NES/HdPacks/HdAudioDevice.obj \
	$(PROJ)/Core/NES/HdPacks/HdNesPack.obj \
	$(PROJ)/Core/NES/HdPacks/HdNesPpu.obj \
	$(PROJ)/Core/NES/HdPacks/HdPackBuilder.obj \
	$(PROJ)/Core/NES/HdPacks/HdPackLoader.obj \
	$(PROJ)/Core/NES/HdPacks/HdVideoFilter.obj \
	$(PROJ)/Core/NES/HdPacks/OggMixer.obj \
	$(PROJ)/Core/NES/HdPacks/OggReader.obj \
	$(PROJ)/Core/NES/Loaders/FdsLoader.obj \
	$(PROJ)/Core/NES/Loaders/iNesLoader.obj \
	$(PROJ)/Core/NES/Loaders/NsfLoader.obj \
	$(PROJ)/Core/NES/Loaders/RomLoader.obj \
	$(PROJ)/Core/NES/Loaders/StudyBoxLoader.obj \
	$(PROJ)/Core/NES/Loaders/UnifLoader.obj \
	$(PROJ)/Core/NES/Mappers/FDS/Fds.obj \
	$(PROJ)/Core/NES/Mappers/FDS/FdsAudio.obj \
	$(PROJ)/Core/NES/Mappers/FDS/FdsInputButtons.obj \
	$(PROJ)/Core/NES/Mappers/NSF/NsfMapper.obj \
	$(PROJ)/Core/NES/Mappers/VsSystem/VsControlManager.obj \
	$(PROJ)/Core/NES/BaseMapper.obj \
	$(PROJ)/Core/NES/BaseNesPpu.obj \
	$(PROJ)/Core/NES/BisqwitNtscFilter.obj \
	$(PROJ)/Core/NES/GameDatabase.obj \
	$(PROJ)/Core/NES/MapperFactory.obj \
	$(PROJ)/Core/NES/NesConsole.obj \
	$(PROJ)/Core/NES/NesControlManager.obj \
	$(PROJ)/Core/NES/NesCpu.obj \
	$(PROJ)/Core/NES/NesDefaultVideoFilter.obj \
	$(PROJ)/Core/NES/NesHeader.obj \
	$(PROJ)/Core/NES/NesMemoryManager.obj \
	$(PROJ)/Core/NES/NesNtscFilter.obj \
	$(PROJ)/Core/NES/NesPpu.obj \
	$(PROJ)/Core/NES/NesSoundMixer.obj

CORE_SMS_OBJS = \
	$(PROJ)/Core/SMS/Carts/SmsCart.obj \
	$(PROJ)/Core/SMS/SmsBiosMapper.obj \
	$(PROJ)/Core/SMS/SmsConsole.obj \
	$(PROJ)/Core/SMS/SmsControlManager.obj \
	$(PROJ)/Core/SMS/SmsCpu.obj \
	$(PROJ)/Core/SMS/SmsFmAudio.obj \
	$(PROJ)/Core/SMS/SmsMemoryManager.obj \
	$(PROJ)/Core/SMS/SmsNtscFilter.obj \
	$(PROJ)/Core/SMS/SmsPsg.obj \
	$(PROJ)/Core/SMS/SmsVdp.obj

CORE_PCE_OBJS = \
	$(PROJ)/Core/PCE/CdRom/PceAdpcm.obj \
	$(PROJ)/Core/PCE/CdRom/PceArcadeCard.obj \
	$(PROJ)/Core/PCE/CdRom/PceAudioFader.obj \
	$(PROJ)/Core/PCE/CdRom/PceCdAudioPlayer.obj \
	$(PROJ)/Core/PCE/CdRom/PceCdRom.obj \
	$(PROJ)/Core/PCE/CdRom/PceScsiBus.obj \
	$(PROJ)/Core/PCE/Input/PceTurboTap.obj \
	$(PROJ)/Core/PCE/PceConsole.obj \
	$(PROJ)/Core/PCE/PceControlManager.obj \
	$(PROJ)/Core/PCE/PceCpu.obj \
	$(PROJ)/Core/PCE/PceCpu.Instructions.obj \
	$(PROJ)/Core/PCE/PceMemoryManager.obj \
	$(PROJ)/Core/PCE/PceNtscFilter.obj \
	$(PROJ)/Core/PCE/PcePsg.obj \
	$(PROJ)/Core/PCE/PcePsgChannel.obj \
	$(PROJ)/Core/PCE/PceSf2RomMapper.obj \
	$(PROJ)/Core/PCE/PceTimer.obj \
	$(PROJ)/Core/PCE/PceVce.obj \
	$(PROJ)/Core/PCE/PceVdc.obj \
	$(PROJ)/Core/PCE/PceVpc.obj

CORE_GAMEBOY_OBJS = \
	$(PROJ)/Core/Gameboy/APU/GbApu.obj \
	$(PROJ)/Core/Gameboy/APU/GbNoiseChannel.obj \
	$(PROJ)/Core/Gameboy/APU/GbSquareChannel.obj \
	$(PROJ)/Core/Gameboy/APU/GbWaveChannel.obj \
	$(PROJ)/Core/Gameboy/Gameboy.obj \
	$(PROJ)/Core/Gameboy/GbControlManager.obj \
	$(PROJ)/Core/Gameboy/GbCpu.obj \
	$(PROJ)/Core/Gameboy/GbDefaultVideoFilter.obj \
	$(PROJ)/Core/Gameboy/GbDmaController.obj \
	$(PROJ)/Core/Gameboy/GbMemoryManager.obj \
	$(PROJ)/Core/Gameboy/GbPpu.obj \
	$(PROJ)/Core/Gameboy/GbTimer.obj

CORE_SNES_OBJS = \
	$(PROJ)/Core/SNES/Coprocessors/BSX/BsxCart.obj \
	$(PROJ)/Core/SNES/Coprocessors/BSX/BsxMemoryPack.obj \
	$(PROJ)/Core/SNES/Coprocessors/BSX/BsxSatellaview.obj \
	$(PROJ)/Core/SNES/Coprocessors/BSX/BsxStream.obj \
	$(PROJ)/Core/SNES/Coprocessors/CX4/Cx4.obj \
	$(PROJ)/Core/SNES/Coprocessors/CX4/Cx4.Instructions.obj \
	$(PROJ)/Core/SNES/Coprocessors/DSP/NecDsp.obj \
	$(PROJ)/Core/SNES/Coprocessors/GSU/Gsu.obj \
	$(PROJ)/Core/SNES/Coprocessors/GSU/Gsu.Instructions.obj \
	$(PROJ)/Core/SNES/Coprocessors/MSU1/Msu1.obj \
	$(PROJ)/Core/SNES/Coprocessors/OBC1/Obc1.obj \
	$(PROJ)/Core/SNES/Coprocessors/SA1/Sa1.obj \
	$(PROJ)/Core/SNES/Coprocessors/SA1/Sa1Cpu.obj \
	$(PROJ)/Core/SNES/Coprocessors/SDD1/Sdd1.obj \
	$(PROJ)/Core/SNES/Coprocessors/SDD1/Sdd1Decomp.obj \
	$(PROJ)/Core/SNES/Coprocessors/SDD1/Sdd1Mmc.obj \
	$(PROJ)/Core/SNES/Coprocessors/SGB/SuperGameboy.obj \
	$(PROJ)/Core/SNES/Coprocessors/SPC7110/Rtc4513.obj \
	$(PROJ)/Core/SNES/Coprocessors/SPC7110/Spc7110.obj \
	$(PROJ)/Core/SNES/Coprocessors/SPC7110/Spc7110Decomp.obj \
	$(PROJ)/Core/SNES/DSP/Dsp.obj \
	$(PROJ)/Core/SNES/DSP/DspVoice.obj \
	$(PROJ)/Core/SNES/Input/Multitap.obj \
	$(PROJ)/Core/SNES/Input/SnesController.obj \
	$(PROJ)/Core/SNES/AluMulDiv.obj \
	$(PROJ)/Core/SNES/BaseCartridge.obj \
	$(PROJ)/Core/SNES/InternalRegisters.obj \
	$(PROJ)/Core/SNES/MemoryMappings.obj \
	$(PROJ)/Core/SNES/RegisterHandlerB.obj \
	$(PROJ)/Core/SNES/SnesConsole.obj \
	$(PROJ)/Core/SNES/SnesControlManager.obj \
	$(PROJ)/Core/SNES/SnesCpu.obj \
	$(PROJ)/Core/SNES/SnesDefaultVideoFilter.obj \
	$(PROJ)/Core/SNES/SnesDmaController.obj \
	$(PROJ)/Core/SNES/SnesMemoryManager.obj \
	$(PROJ)/Core/SNES/SnesNtscFilter.obj \
	$(PROJ)/Core/SNES/SnesPpu.obj \
	$(PROJ)/Core/SNES/Spc.obj \
	$(PROJ)/Core/SNES/Spc.Instructions.obj

SHIM_OBJS = \
	Shim/Audio/SoundMixer.obj \
	Shim/Debugger/Debugger.obj \
	Shim/Debugger/PpuTools.obj \
	Shim/Debugger/ScriptManager.obj \
	Shim/Movies/MovieManager.obj \
	Shim/Movies/MovieRecorder.obj \
	Shim/SNES/Debugger/SnesPpuTools.obj \
	Shim/Utilities/FolderUtilities.obj \
	Shim/Video/VideoDecoder.obj \
	Shim/Video/VideoRenderer.obj \
	Shim/BatteryManager.obj \
	Shim/Emulator.obj \
	Shim/SaveStateManager.obj \
	Shim/ShimKeyManager.obj

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
	core.obj \
	settings.obj

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4100 \
	/wd4127 \
	/wd4189 \
	/wd4244 \
	/wd4245 \
	/wd4310 \
	/wd4456 \
	/wd4457 \
	/wd4701 \
	/wd4706 \
	/wd4723 \
	/volatile:iso \
	/nologo

INCLUDES = \
	-I.. \
	-I$(PROJ) \
	-I$(PROJ)/Core \
	-I$(PROJ)/Utilities

DEFS = \
	-DUNICODE \
	-DCORE_EXPORT

LINK_FLAGS = \
	/nodefaultlib \
	/nologo \
	/dll

LIBS = \
	libvcruntime.lib \
	libucrt.lib \
	libcmt.lib \
	libcpmt.lib \
	kernel32.lib \
	ole32.lib \
	oleaut32.lib \
	ws2_32.lib

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CPPFLAGS = $(CFLAGS) -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING /EHsc /std:c++17

all: clean clear $(OBJS)
	link /out:$(NAME).dll $(LINK_FLAGS) *.obj $(LIBS)

clean:
	@-del /q *.obj 2>nul
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul

clear:
	@cls
