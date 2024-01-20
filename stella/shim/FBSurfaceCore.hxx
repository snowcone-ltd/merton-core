#pragma once

#include "bspf.hxx"
#include "FBSurface.hxx"

class FBSurfaceCore : public FBSurface {
	public:
		FBSurfaceCore(uInt32 width, uInt32 height) : myWidth{width}, myHeight{height},
			myPixelData{make_unique<uInt32[]>(myWidth * myHeight)}
		{
			myPixels = myPixelData.get();
			myPitch = myWidth;
		}

		~FBSurfaceCore() override {}

		void fillRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h, ColorId color) override {}

		uInt32 width() const override {return myWidth;}
		uInt32 height() const override {return myHeight;}

		const Common::Rect& srcRect() const override {return mySrcGUIR;}
		const Common::Rect& dstRect() const override {return myDstGUIR;}
		void setSrcPos(uInt32 x, uInt32 y) override {}
		void setSrcSize(uInt32 w, uInt32 h) override {}
		void setSrcRect(const Common::Rect& r) override {}
		void setDstPos(uInt32 x, uInt32 y) override {}
		void setDstSize(uInt32 w, uInt32 h) override {}
		void setDstRect(const Common::Rect& r) override {}

		void setVisible(bool visible) override {}

		void translateCoords(Int32& x, Int32& y) const override {}
		bool render() override {return true;}
		void invalidate() override {}
		void invalidateRect(uInt32, uInt32, uInt32, uInt32) override {}
		void reload() override {}
		void resize(uInt32 width, uInt32 height) override {}
		void setScalingInterpolation(ScalingInterpolation) override {}

	protected:
		void applyAttributes() override {}

	private:
		uInt32 myWidth{0}, myHeight{0};
		unique_ptr<uInt32[]> myPixelData;
		Common::Rect mySrcGUIR, myDstGUIR;

	private:
		FBSurfaceCore() = delete;
		FBSurfaceCore(const FBSurfaceCore&) = delete;
		FBSurfaceCore(FBSurfaceCore&&) = delete;
		FBSurfaceCore& operator=(const FBSurfaceCore&) = delete;
		FBSurfaceCore& operator=(FBSurfaceCore&&) = delete;
};
