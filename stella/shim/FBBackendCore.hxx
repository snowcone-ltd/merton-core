#pragma once

class OSystem;

#include "bspf.hxx"
#include "FBBackend.hxx"
#include "FBSurfaceCore.hxx"

class FBBackendCore : public FBBackend {
	public:
		explicit FBBackendCore(OSystem&) {}
		~FBBackendCore() override {}

	protected:
		uInt32 mapRGB(uInt8 r, uInt8 g, uInt8 b) const override
		{
			return (r << 16) | (g << 8) | b;
		}

		uInt32 mapRGBA(uInt8 r, uInt8 g, uInt8 b, uInt8 a) const override
		{
			return (a << 24) | (r << 16) | (g << 8) | b;
		}

		void queryHardware(vector<Common::Size>& fullscreenRes,
			vector<Common::Size>& windowedRes, VariantList& renderers) override
		{
			fullscreenRes.emplace_back(1920, 1080);
			windowedRes.emplace_back(1920, 1080);

			VarList::push_back(renderers, "software", "Software");
		}

		unique_ptr<FBSurface> createSurface(uInt32 w, uInt32 h,
			ScalingInterpolation, const uInt32*) const override
		{
			return make_unique<FBSurfaceCore>(w, h);
		}

		string about() const override {return "Video system: core";}

		int scaleX(int x) const override {return x;}
		int scaleY(int y) const override {return y;}
		void setTitle(string_view) override {}
		void showCursor(bool) override {}
		bool fullScreen() const override {return true;}
		void getRGB(uInt32, uInt8*, uInt8*, uInt8*) const override {}
		void getRGBA(uInt32, uInt8*, uInt8*, uInt8*, uInt8*) const override {}
		void readPixels(uInt8*, size_t, const Common::Rect&) const override {}
		bool isCurrentWindowPositioned() const override {return true;}
		Common::Point getCurrentWindowPos() const override {return Common::Point{};}
		Int32 getCurrentDisplayIndex() const override {return 0;}
		void clear() override {}
		bool setVideoMode(const VideoModeHandler::Mode&,
			int, const Common::Point&) override {return true;}
		void grabMouse(bool) override {}
		void renderToScreen() override {}
		int refreshRate() const override {return 0;}

	private:
		FBBackendCore() = delete;
		FBBackendCore(const FBBackendCore&) = delete;
		FBBackendCore(FBBackendCore&&) = delete;
		FBBackendCore& operator=(const FBBackendCore&) = delete;
		FBBackendCore& operator=(FBBackendCore&&) = delete;
};
