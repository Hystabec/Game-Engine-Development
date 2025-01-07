#pragma once
//#include "SDL.h"
#include <map>
#include <vector>
#include <SDL.h>

enum KEYS_PRESSED_LIST
{
	KEY_ESCAPE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
	KEY_GRAVE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_PLUS, KEY_BACKSPACE,
	KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_ENTER,
	KEY_CAPS_LOCK, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_COLON, KEY_AT, KEY_HASH,
	KEY_LEFT_SHIFT, KEY_BACK_SLASH, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_FULL_STOP, KEY_FOWARD_SLASH, KEY_RIGHT_SHIFT,
	KEY_LEFT_CONTROL, KEY_LEFT_ALT, KEY_SPACE, KEY_RIGHT_CONTROL,
	SIZE_OF_KEYS_PRESSED_ENUM
};

//make the input system a singleton
//when a key is pressed it will fire an event
	
/// @brief This is a singleton that will handle all of the inputs for the game and will fire events through the EventManager when a key changes state
class Input
{
public:
	void Update(void);
	bool KeyIsPressed(KEYS_PRESSED_LIST key);
	void setUp();
	bool windowCloseEventCalled();

	Input();
	~Input();
private:
	void HandleInputWithEvents();
	SDL_Event m_event;
	bool m_keysPressed[SIZE_OF_KEYS_PRESSED_ENUM];
	bool m_keysPressedPrevious[SIZE_OF_KEYS_PRESSED_ENUM];
	bool windowCloseCalled = false;
};	