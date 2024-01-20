PROJ = stella
NAME = stella

OBJS = \
	core.obj \
	$(PROJ)/src/common/audio/ConvolutionBuffer.obj \
	$(PROJ)/src/common/audio/HighPass.obj \
	$(PROJ)/src/common/audio/LanczosResampler.obj \
	$(PROJ)/src/common/audio/SimpleResampler.obj \
	$(PROJ)/src/common/repository/sqlite/AbstractKeyValueRepositorySqlite.obj \
	$(PROJ)/src/common/repository/sqlite/CompositeKeyValueRepositorySqlite.obj \
	$(PROJ)/src/common/repository/sqlite/KeyValueRepositorySqlite.obj \
	$(PROJ)/src/common/repository/sqlite/SqliteDatabase.obj \
	$(PROJ)/src/common/repository/sqlite/SqliteStatement.obj \
	$(PROJ)/src/common/repository/sqlite/SqliteTransaction.obj \
	$(PROJ)/src/common/repository/sqlite/StellaDb.obj \
	$(PROJ)/src/common/repository/CompositeKeyValueRepository.obj \
	$(PROJ)/src/common/repository/CompositeKVRJsonAdapter.obj \
	$(PROJ)/src/common/repository/KeyValueRepositoryConfigfile.obj \
	$(PROJ)/src/common/repository/KeyValueRepositoryJsonFile.obj \
	$(PROJ)/src/common/repository/KeyValueRepositoryPropertyFile.obj \
	$(PROJ)/src/common/tv_filters/AtariNTSC.obj \
	$(PROJ)/src/common/tv_filters/NTSCFilter.obj \
	$(PROJ)/src/common/AudioQueue.obj \
	$(PROJ)/src/common/AudioSettings.obj \
	$(PROJ)/src/common/Base.obj \
	$(PROJ)/src/common/Bezel.obj \
	$(PROJ)/src/common/DevSettingsHandler.obj \
	$(PROJ)/src/common/FpsMeter.obj \
	$(PROJ)/src/common/JoyMap.obj \
	$(PROJ)/src/common/KeyMap.obj \
	$(PROJ)/src/common/Logger.obj \
	$(PROJ)/src/common/MouseControl.obj \
	$(PROJ)/src/common/PaletteHandler.obj \
	$(PROJ)/src/common/PhosphorHandler.obj \
	$(PROJ)/src/common/PhysicalJoystick.obj \
	$(PROJ)/src/common/PJoystickHandler.obj \
	$(PROJ)/src/common/PKeyboardHandler.obj \
	$(PROJ)/src/common/RewindManager.obj \
	$(PROJ)/src/common/StaggeredLogger.obj \
	$(PROJ)/src/common/StateManager.obj \
	$(PROJ)/src/common/TimerManager.obj \
	$(PROJ)/src/common/VideoModeHandler.obj \
	$(PROJ)/src/emucore/tia/frame-manager/AbstractFrameManager.obj \
	$(PROJ)/src/emucore/tia/frame-manager/FrameLayoutDetector.obj \
	$(PROJ)/src/emucore/tia/frame-manager/FrameManager.obj \
	$(PROJ)/src/emucore/tia/frame-manager/JitterEmulation.obj \
	$(PROJ)/src/emucore/tia/AnalogReadout.obj \
	$(PROJ)/src/emucore/tia/Audio.obj \
	$(PROJ)/src/emucore/tia/AudioChannel.obj \
	$(PROJ)/src/emucore/tia/Background.obj \
	$(PROJ)/src/emucore/tia/Ball.obj \
	$(PROJ)/src/emucore/tia/DrawCounterDecodes.obj \
	$(PROJ)/src/emucore/tia/LatchedInput.obj \
	$(PROJ)/src/emucore/tia/Missile.obj \
	$(PROJ)/src/emucore/tia/Player.obj \
	$(PROJ)/src/emucore/tia/Playfield.obj \
	$(PROJ)/src/emucore/tia/TIA.obj \
	$(PROJ)/src/emucore/AtariVox.obj \
	$(PROJ)/src/emucore/Bankswitch.obj \
	$(PROJ)/src/emucore/Booster.obj \
	$(PROJ)/src/emucore/Cart.obj \
	$(PROJ)/src/emucore/Cart0FA0.obj \
	$(PROJ)/src/emucore/Cart2K.obj \
	$(PROJ)/src/emucore/Cart03E0.obj \
	$(PROJ)/src/emucore/Cart3E.obj \
	$(PROJ)/src/emucore/Cart3EPlus.obj \
	$(PROJ)/src/emucore/Cart3EX.obj \
	$(PROJ)/src/emucore/Cart3F.obj \
	$(PROJ)/src/emucore/Cart4A50.obj \
	$(PROJ)/src/emucore/Cart4K.obj \
	$(PROJ)/src/emucore/Cart4KSC.obj \
	$(PROJ)/src/emucore/Cart0840.obj \
	$(PROJ)/src/emucore/CartAR.obj \
	$(PROJ)/src/emucore/CartARM.obj \
	$(PROJ)/src/emucore/CartBF.obj \
	$(PROJ)/src/emucore/CartBFSC.obj \
	$(PROJ)/src/emucore/CartBUS.obj \
	$(PROJ)/src/emucore/CartCDF.obj \
	$(PROJ)/src/emucore/CartCM.obj \
	$(PROJ)/src/emucore/CartCreator.obj \
	$(PROJ)/src/emucore/CartCTY.obj \
	$(PROJ)/src/emucore/CartCV.obj \
	$(PROJ)/src/emucore/CartDetector.obj \
	$(PROJ)/src/emucore/CartDF.obj \
	$(PROJ)/src/emucore/CartDFSC.obj \
	$(PROJ)/src/emucore/CartDPC.obj \
	$(PROJ)/src/emucore/CartDPCPlus.obj \
	$(PROJ)/src/emucore/CartE0.obj \
	$(PROJ)/src/emucore/CartE7.obj \
	$(PROJ)/src/emucore/CartEF.obj \
	$(PROJ)/src/emucore/CartEFSC.obj \
	$(PROJ)/src/emucore/CartEnhanced.obj \
	$(PROJ)/src/emucore/CartF0.obj \
	$(PROJ)/src/emucore/CartF4.obj \
	$(PROJ)/src/emucore/CartF4SC.obj \
	$(PROJ)/src/emucore/CartF6.obj \
	$(PROJ)/src/emucore/CartF6SC.obj \
	$(PROJ)/src/emucore/CartF8.obj \
	$(PROJ)/src/emucore/CartF8SC.obj \
	$(PROJ)/src/emucore/CartFA.obj \
	$(PROJ)/src/emucore/CartFA2.obj \
	$(PROJ)/src/emucore/CartFC.obj \
	$(PROJ)/src/emucore/CartFE.obj \
	$(PROJ)/src/emucore/CartGL.obj \
	$(PROJ)/src/emucore/CartMDM.obj \
	$(PROJ)/src/emucore/CartMVC.obj \
	$(PROJ)/src/emucore/CartSB.obj \
	$(PROJ)/src/emucore/CartTVBoy.obj \
	$(PROJ)/src/emucore/CartUA.obj \
	$(PROJ)/src/emucore/CartWD.obj \
	$(PROJ)/src/emucore/CartX07.obj \
	$(PROJ)/src/emucore/CompuMate.obj \
	$(PROJ)/src/emucore/Console.obj \
	$(PROJ)/src/emucore/Control.obj \
	$(PROJ)/src/emucore/ControllerDetector.obj \
	$(PROJ)/src/emucore/DispatchResult.obj \
	$(PROJ)/src/emucore/Driving.obj \
	$(PROJ)/src/emucore/EmulationTiming.obj \
	$(PROJ)/src/emucore/EmulationWorker.obj \
	$(PROJ)/src/emucore/EventHandler.obj \
	$(PROJ)/src/emucore/FBSurface.obj \
	$(PROJ)/src/emucore/FrameBuffer.obj \
	$(PROJ)/src/emucore/FSNode.obj \
	$(PROJ)/src/emucore/Genesis.obj \
	$(PROJ)/src/emucore/GlobalKeyHandler.obj \
	$(PROJ)/src/emucore/Joy2BPlus.obj \
	$(PROJ)/src/emucore/Joystick.obj \
	$(PROJ)/src/emucore/Keyboard.obj \
	$(PROJ)/src/emucore/KidVid.obj \
	$(PROJ)/src/emucore/Lightgun.obj \
	$(PROJ)/src/emucore/M6502.obj \
	$(PROJ)/src/emucore/M6532.obj \
	$(PROJ)/src/emucore/MD5.obj \
	$(PROJ)/src/emucore/MindLink.obj \
	$(PROJ)/src/emucore/MT24LC256.obj \
	$(PROJ)/src/emucore/OSystem.obj \
	$(PROJ)/src/emucore/OSystemStandalone.obj \
	$(PROJ)/src/emucore/Paddles.obj \
	$(PROJ)/src/emucore/PlusROM.obj \
	$(PROJ)/src/emucore/PointingDevice.obj \
	$(PROJ)/src/emucore/ProfilingRunner.obj \
	$(PROJ)/src/emucore/Props.obj \
	$(PROJ)/src/emucore/PropsSet.obj \
	$(PROJ)/src/emucore/QuadTari.obj \
	$(PROJ)/src/emucore/SaveKey.obj \
	$(PROJ)/src/emucore/Serializer.obj \
	$(PROJ)/src/emucore/Settings.obj \
	$(PROJ)/src/emucore/Switches.obj \
	$(PROJ)/src/emucore/System.obj \
	$(PROJ)/src/emucore/Thumbulator.obj \
	$(PROJ)/src/emucore/TIASurface.obj \
	$(PROJ)/src/lib/sqlite/sqlite3.obj \
	$(PROJ)/src/os/windows/FSNodeWINDOWS.obj \
	$(PROJ)/src/os/windows/OSystemWINDOWS.obj \
	$(PROJ)/src/os/windows/SerialPortWINDOWS.obj

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4100 \
	/wd4244 \
	/volatile:iso \
	/nologo

INCLUDES = \
	-Ishim \
	-I$(PROJ)/src/common \
	-I$(PROJ)/src/common\tv_filters \
	-I$(PROJ)/src/common\repository\sqlite \
	-I$(PROJ)/src/emucore \
	-I$(PROJ)/src/emucore/tia \
	-I$(PROJ)/src/emucore/tia/frame-manager \
	-I$(PROJ)/src/lib/json \
	-I$(PROJ)/src/lib/sqlite \
	-I$(PROJ)/src/os/windows

DEFS = \
	-DCORE_EXPORT \
	-DBSPF_WINDOWS \
	-DNOMINMAX \
	-DSOUND_SUPPORT

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
	advapi32.lib \
	shell32.lib

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CXXFLAGS = $(CFLAGS) /EHsc /std:c++17

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
