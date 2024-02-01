!INCLUDE ..\..\windows-shared

PROJ = parallel-rdp-standalone

O = obj

OBJS = \
	parallel-rdp.$(O) \
	$(PROJ)/parallel-rdp/command_ring.$(O) \
	$(PROJ)/parallel-rdp/rdp_device.$(O) \
	$(PROJ)/parallel-rdp/rdp_dump_write.$(O) \
	$(PROJ)/parallel-rdp/rdp_renderer.$(O) \
	$(PROJ)/parallel-rdp/video_interface.$(O) \
	$(PROJ)/util/aligned_alloc.$(O) \
	$(PROJ)/util/arena_allocator.$(O) \
	$(PROJ)/util/logging.$(O) \
	$(PROJ)/util/thread_id.$(O) \
	$(PROJ)/util/thread_name.$(O) \
	$(PROJ)/util/timeline_trace_file.$(O) \
	$(PROJ)/util/timer.$(O) \
	$(PROJ)/volk/volk.$(O) \
	$(PROJ)/vulkan/texture/texture_format.$(O) \
	$(PROJ)/vulkan/buffer.$(O) \
	$(PROJ)/vulkan/buffer_pool.$(O) \
	$(PROJ)/vulkan/command_buffer.$(O) \
	$(PROJ)/vulkan/command_pool.$(O) \
	$(PROJ)/vulkan/context.$(O) \
	$(PROJ)/vulkan/cookie.$(O) \
	$(PROJ)/vulkan/descriptor_set.$(O) \
	$(PROJ)/vulkan/device.$(O) \
	$(PROJ)/vulkan/event_manager.$(O) \
	$(PROJ)/vulkan/fence.$(O) \
	$(PROJ)/vulkan/fence_manager.$(O) \
	$(PROJ)/vulkan/image.$(O) \
	$(PROJ)/vulkan/indirect_layout.$(O) \
	$(PROJ)/vulkan/memory_allocator.$(O) \
	$(PROJ)/vulkan/pipeline_event.$(O) \
	$(PROJ)/vulkan/query_pool.$(O) \
	$(PROJ)/vulkan/render_pass.$(O) \
	$(PROJ)/vulkan/sampler.$(O) \
	$(PROJ)/vulkan/semaphore.$(O) \
	$(PROJ)/vulkan/semaphore_manager.$(O) \
	$(PROJ)/vulkan/shader.$(O) \

DEFS = \
	-D_CRT_SECURE_NO_WARNINGS \
	-DVK_USE_PLATFORM_WIN32_KHR

INCLUDES = \
	-I../../../libmatoya/src \
	-I../mupen64plus-core/src \
	-I$(PROJ)/parallel-rdp \
	-I$(PROJ)/util \
	-I$(PROJ)/volk \
	-I$(PROJ)/vulkan \
	-I$(PROJ)/vulkan-headers/include \

FLAGS = \
	/W4 \
	/MT \
	/MP \
	/wd4018 \
	/wd4100 \
	/wd4244 \
	/wd4245 \
	/wd4267 \
	/wd4389 \
	/wd4458 \
	/wd4701 \
	/wd4201 \
	/volatile:iso \
	/nologo

LINK_FLAGS = \
	/nologo

LIBS = \
	..\..\..\libmatoya\bin\windows\x64\matoya.lib \
	advapi32.lib \
	bcrypt.lib \
	crypt32.lib \
	d3d11.lib \
	d3d12.lib \
	dxgi.lib \
	dxguid.lib \
	gdi32.lib \
	hid.lib \
	imm32.lib \
	ole32.lib \
	shcore.lib \
	shell32.lib \
	shlwapi.lib \
	user32.lib \
	userenv.lib \
	windowscodecs.lib \
	winhttp.lib \
	winmm.lib \
	ws2_32.lib \
	xinput.lib

!IFDEF DEBUG
FLAGS = $(FLAGS) /Ob0 /Zi /Oy-
LINK_FLAGS = $(LINK_FLAGS) /debug
!ELSE
FLAGS = $(FLAGS) /O2 /GS- /Gw /GL
LINK_FLAGS = $(LINK_FLAGS) /LTCG
!ENDIF

CFLAGS = $(INCLUDES) $(DEFS) $(FLAGS)
CPPFLAGS = $(INCLUDES) $(DEFS) $(FLAGS) /EHsc

all: clean clear $(OBJS)
	link /out:rdp.exe $(LINK_FLAGS) *.obj $(LIBS)

clean:
	@-del /q *.obj 2>nul
	@-del /q *.exe 2>nul
	@-del /q *.lib 2>nul
	@-del /q *.dll 2>nul
	@-del /q *.ilk 2>nul
	@-del /q *.pdb 2>nul
	@-del /q *.exp 2>nul

clear:
	@cls
