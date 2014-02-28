#ifndef INPUT_H
#define INPUT_H

namespace cpp3ds {

	enum Button {
		BUTTON_A      = 1,
		BUTTON_B      = 2,
		BUTTON_X      = 1024,
		BUTTON_Y      = 2048,
		BUTTON_UP     = 64,
		BUTTON_DOWN   = 128,
		BUTTON_LEFT   = 32,
		BUTTON_RIGHT  = 16,
		BUTTON_L1     = 512,
		BUTTON_R1     = 256,
		BUTTON_START  = 8,
		BUTTON_SELECT = 4
	};

	enum Event {
		EVENT_DOWN,
		EVENT_UP,
		EVENT_PRESS,
		EVENT_HOLD
	};

	typedef void (*InputCallback)(Button button);

	class Input {
	protected:
		int getHID();
		int last_hid;
	public:
		static float slider;
		static bool isDown(Button button);
		static void update3DSlider();
		static float get3DSlider(){ return Input::slider; }
		// int bind(Button button, Event event);
		// void unbind(int binding);
		// void unbindAll();
		// void unbindAll(Button button);
	};

}

#endif
