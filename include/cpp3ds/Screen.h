#ifdef SIMULATION
#include <sim3ds/Screen.h>
#endif

#ifndef SCREEN_H
#define SCREEN_H

#include <cpp3ds/Color.h>

namespace cpp3ds {

	class Drawable; // Forward-declaration for cyclic reference

	class Screen {
	private:
		void setPixelAddress(int addr, Color color);
	protected:
		bool buffer;
		int frame1, frame2;
		int width, height;
	public:
		Screen(int addr1, int addr2, int w, int h):
			frame1(addr1), frame2(addr2), width(w), height(h) {}
		int getWidth(){ return width; }
		int getHeight(){ return height; }
		void setPixel(int x, int y, Color color);
		void clear(Color color = {0,0,0});
		void display();
		virtual void draw(Drawable& obj, float x = 0, float y = 0, bool use3D = false);
	};
}

#endif
