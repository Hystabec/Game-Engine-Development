#include "Input.h"
#include "EventManager.h"

#include <backends/imgui_impl_sdl.h>
#include "Profiler.h"

//#include <iostream>
//#include <chrono>

void Input::Update(void)
{
	//auto start = std::chrono::high_resolution_clock::now();

	profiler->startProfile("Input Update");

	while (SDL_PollEvent(&m_event) != NULL)
	{
		ImGui_ImplSDL2_ProcessEvent(&m_event);

		if (m_event.type == SDL_KEYDOWN)
		{
			SDL_Keycode keyPressed = m_event.key.keysym.sym;

			switch (keyPressed)
			{
			case SDLK_ESCAPE:
				m_keysPressed[KEY_ESCAPE] = true;
				break;
			case SDLK_F1:
				m_keysPressed[KEY_F1] = true;
				break;
			case SDLK_F2:
				m_keysPressed[KEY_F2] = true;
				break;
			case SDLK_F3:
				m_keysPressed[KEY_F3] = true;
				break;
			case SDLK_F4:
				m_keysPressed[KEY_F4] = true;
				break;
			case SDLK_F5:
				m_keysPressed[KEY_F5] = true;
				break;
			case SDLK_F6:
				m_keysPressed[KEY_F6] = true;
				break;
			case SDLK_F7:
				m_keysPressed[KEY_F7] = true;
				break;
			case SDLK_F8:
				m_keysPressed[KEY_F8] = true;
				break;
			case SDLK_F9:
				m_keysPressed[KEY_F9] = true;
				break;
			case SDLK_F10:
				m_keysPressed[KEY_F10] = true;
				break;
			case SDLK_F11:
				m_keysPressed[KEY_F11] = true;
				break;
			case SDLK_F12:
				m_keysPressed[KEY_F12] = true;
				break;
			case SDLK_BACKQUOTE:
				m_keysPressed[KEY_GRAVE] = true;
				break;
			case SDLK_1:
				m_keysPressed[KEY_1] = true;
				break;
			case SDLK_2:
				m_keysPressed[KEY_2] = true;
				break;
			case SDLK_3:
				m_keysPressed[KEY_3] = true;
				break;
			case SDLK_4:
				m_keysPressed[KEY_4] = true;
				break;
			case SDLK_5:
				m_keysPressed[KEY_5] = true;
				break;
			case SDLK_6:
				m_keysPressed[KEY_6] = true;
				break;
			case SDLK_7:
				m_keysPressed[KEY_7] = true;
				break;
			case SDLK_8:
				m_keysPressed[KEY_8] = true;
				break;
			case SDLK_9:
				m_keysPressed[KEY_9] = true;
				break;
			case SDLK_0:
				m_keysPressed[KEY_0] = true;
				break;
			case SDLK_MINUS:
				m_keysPressed[KEY_MINUS] = true;
				break;
			case SDLK_PLUS:
				m_keysPressed[KEY_PLUS] = true;
				break;
			case SDLK_BACKSPACE:
				m_keysPressed[KEY_BACKSPACE] = true;
				break;
			case SDLK_TAB:
				m_keysPressed[KEY_TAB] = true;
				break;
			case SDLK_q:
				m_keysPressed[KEY_Q] = true;
				break;
			case SDLK_w:
				m_keysPressed[KEY_W] = true;
				break;
			case SDLK_e:
				m_keysPressed[KEY_E] = true;
				break;
			case SDLK_r:
				m_keysPressed[KEY_R] = true;
				break;
			case SDLK_t:
				m_keysPressed[KEY_T] = true;
				break;
			case SDLK_y:
				m_keysPressed[KEY_Y] = true;
				break;
			case SDLK_u:
				m_keysPressed[KEY_U] = true;
				break;
			case SDLK_i:
				m_keysPressed[KEY_I] = true;
				break;
			case SDLK_o:
				m_keysPressed[KEY_O] = true;
				break;
			case SDLK_p:
				m_keysPressed[KEY_P] = true;
				break;
			case SDLK_LEFTBRACKET:
				m_keysPressed[KEY_LEFT_BRACE] = true;
				break;
			case SDLK_RIGHTBRACKET:
				m_keysPressed[KEY_RIGHT_BRACE] = true;
				break;
			case SDLK_KP_ENTER:
				m_keysPressed[KEY_ENTER] = true;
				break;
			case SDLK_CAPSLOCK:
				m_keysPressed[KEY_CAPS_LOCK] = true;
				break;
			case SDLK_a:
				m_keysPressed[KEY_A] = true;
				break;
			case SDLK_s:
				m_keysPressed[KEY_S] = true;
				break;
			case SDLK_d:
				m_keysPressed[KEY_D] = true;
				break;
			case SDLK_f:
				m_keysPressed[KEY_F] = true;
				break;
			case SDLK_g:
				m_keysPressed[KEY_G] = true;
				break;
			case SDLK_h:
				m_keysPressed[KEY_H] = true;
				break;
			case SDLK_j:
				m_keysPressed[KEY_J] = true;
				break;
			case SDLK_k:
				m_keysPressed[KEY_K] = true;
				break;
			case SDLK_l:
				m_keysPressed[KEY_L] = true;
				break;
			case SDLK_COLON:
				m_keysPressed[KEY_COLON] = true;
				break;
			case SDLK_AT:
				m_keysPressed[KEY_AT] = true;
				break;
			case SDLK_HASH:
				m_keysPressed[KEY_HASH] = true;
				break;
			case SDLK_LSHIFT:
				m_keysPressed[KEY_LEFT_SHIFT] = true;
				break;
			case SDLK_QUOTE:
				m_keysPressed[KEY_BACK_SLASH] = true;
				break;
			case SDLK_z:
				m_keysPressed[KEY_Z] = true;
				break;
			case SDLK_x:
				m_keysPressed[KEY_X] = true;
				break;
			case SDLK_c:
				m_keysPressed[KEY_C] = true;
				break;
			case SDLK_v:
				m_keysPressed[KEY_V] = true;
				break;
			case SDLK_b:
				m_keysPressed[KEY_B] = true;
				break;
			case SDLK_n:
				m_keysPressed[KEY_N] = true;
				break;
			case SDLK_m:
				m_keysPressed[KEY_M] = true;
				break;
			case SDLK_COMMA:
				m_keysPressed[KEY_COMMA] = true;
				break;
			case SDLK_PERIOD:
				m_keysPressed[KEY_FULL_STOP] = true;
				break;
			case SDLK_SLASH:
				m_keysPressed[KEY_FOWARD_SLASH] = true;
				break;
			case SDLK_RSHIFT:
				m_keysPressed[KEY_RIGHT_SHIFT] = true;
				break;
			case SDLK_LCTRL:
				m_keysPressed[KEY_LEFT_CONTROL] = true;
				break;
			case SDLK_LALT:
				m_keysPressed[KEY_LEFT_ALT] = true;
				break;
			case SDLK_SPACE:
				m_keysPressed[KEY_SPACE] = true;
				break;
			case SDLK_RCTRL:
				m_keysPressed[KEY_RIGHT_CONTROL] = true;
				break;
			default:
				break;
			}
		}

		else if (m_event.type == SDL_KEYUP)
		{
			SDL_Keycode keyPressed = m_event.key.keysym.sym;

			switch (keyPressed)
			{
			case SDLK_ESCAPE:
				m_keysPressed[KEY_ESCAPE] = false;
				break;
			case SDLK_F1:
				m_keysPressed[KEY_F1] = false;
				break;
			case SDLK_F2:
				m_keysPressed[KEY_F2] = false;
				break;
			case SDLK_F3:
				m_keysPressed[KEY_F3] = false;
				break;
			case SDLK_F4:
				m_keysPressed[KEY_F4] = false;
				break;
			case SDLK_F5:
				m_keysPressed[KEY_F5] = false;
				break;
			case SDLK_F6:
				m_keysPressed[KEY_F6] = false;
				break;
			case SDLK_F7:
				m_keysPressed[KEY_F7] = false;
				break;
			case SDLK_F8:
				m_keysPressed[KEY_F8] = false;
				break;
			case SDLK_F9:
				m_keysPressed[KEY_F9] = false;
				break;
			case SDLK_F10:
				m_keysPressed[KEY_F10] = false;
				break;
			case SDLK_F11:
				m_keysPressed[KEY_F11] = false;
				break;
			case SDLK_F12:
				m_keysPressed[KEY_F12] = false;
				break;
			case SDLK_BACKQUOTE:
				m_keysPressed[KEY_GRAVE] = false;
				break;
			case SDLK_1:
				m_keysPressed[KEY_1] = false;
				break;
			case SDLK_2:
				m_keysPressed[KEY_2] = false;
				break;
			case SDLK_3:
				m_keysPressed[KEY_3] = false;
				break;
			case SDLK_4:
				m_keysPressed[KEY_4] = false;
				break;
			case SDLK_5:
				m_keysPressed[KEY_5] = false;
				break;
			case SDLK_6:
				m_keysPressed[KEY_6] = false;
				break;
			case SDLK_7:
				m_keysPressed[KEY_7] = false;
				break;
			case SDLK_8:
				m_keysPressed[KEY_8] = false;
				break;
			case SDLK_9:
				m_keysPressed[KEY_9] = false;
				break;
			case SDLK_0:
				m_keysPressed[KEY_0] = false;
				break;
			case SDLK_MINUS:
				m_keysPressed[KEY_MINUS] = false;
				break;
			case SDLK_PLUS:
				m_keysPressed[KEY_PLUS] = false;
				break;
			case SDLK_BACKSPACE:
				m_keysPressed[KEY_BACKSPACE] = false;
				break;
			case SDLK_TAB:
				m_keysPressed[KEY_TAB] = false;
				break;
			case SDLK_q:
				m_keysPressed[KEY_Q] = false;
				break;
			case SDLK_w:
				m_keysPressed[KEY_W] = false;
				break;
			case SDLK_e:
				m_keysPressed[KEY_E] = false;
				break;
			case SDLK_r:
				m_keysPressed[KEY_R] = false;
				break;
			case SDLK_t:
				m_keysPressed[KEY_T] = false;
				break;
			case SDLK_y:
				m_keysPressed[KEY_Y] = false;
				break;
			case SDLK_u:
				m_keysPressed[KEY_U] = false;
				break;
			case SDLK_i:
				m_keysPressed[KEY_I] = false;
				break;
			case SDLK_o:
				m_keysPressed[KEY_O] = false;
				break;
			case SDLK_p:
				m_keysPressed[KEY_P] = false;
				break;
			case SDLK_LEFTBRACKET:
				m_keysPressed[KEY_LEFT_BRACE] = false;
				break;
			case SDLK_RIGHTBRACKET:
				m_keysPressed[KEY_RIGHT_BRACE] = false;
				break;
			case SDLK_KP_ENTER:
				m_keysPressed[KEY_ENTER] = false;
				break;
			case SDLK_CAPSLOCK:
				m_keysPressed[KEY_CAPS_LOCK] = false;
				break;
			case SDLK_a:
				m_keysPressed[KEY_A] = false;
				break;
			case SDLK_s:
				m_keysPressed[KEY_S] = false;
				break;
			case SDLK_d:
				m_keysPressed[KEY_D] = false;
				break;
			case SDLK_f:
				m_keysPressed[KEY_F] = false;
				break;
			case SDLK_g:
				m_keysPressed[KEY_G] = false;
				break;
			case SDLK_h:
				m_keysPressed[KEY_H] = false;
				break;
			case SDLK_j:
				m_keysPressed[KEY_J] = false;
				break;
			case SDLK_k:
				m_keysPressed[KEY_K] = false;
				break;
			case SDLK_l:
				m_keysPressed[KEY_L] = false;
				break;
			case SDLK_COLON:
				m_keysPressed[KEY_COLON] = false;
				break;
			case SDLK_AT:
				m_keysPressed[KEY_AT] = false;
				break;
			case SDLK_HASH:
				m_keysPressed[KEY_HASH] = false;
				break;
			case SDLK_LSHIFT:
				m_keysPressed[KEY_LEFT_SHIFT] = false;
				break;
			case SDLK_QUOTE:
				m_keysPressed[KEY_BACK_SLASH] = false;
				break;
			case SDLK_z:
				m_keysPressed[KEY_Z] = false;
				break;
			case SDLK_x:
				m_keysPressed[KEY_X] = false;
				break;
			case SDLK_c:
				m_keysPressed[KEY_C] = false;
				break;
			case SDLK_v:
				m_keysPressed[KEY_V] = false;
				break;
			case SDLK_b:
				m_keysPressed[KEY_B] = false;
				break;
			case SDLK_n:
				m_keysPressed[KEY_N] = false;
				break;
			case SDLK_m:
				m_keysPressed[KEY_M] = false;
				break;
			case SDLK_COMMA:
				m_keysPressed[KEY_COMMA] = false;
				break;
			case SDLK_PERIOD:
				m_keysPressed[KEY_FULL_STOP] = false;
				break;
			case SDLK_SLASH:
				m_keysPressed[KEY_FOWARD_SLASH] = false;
				break;
			case SDLK_RSHIFT:
				m_keysPressed[KEY_RIGHT_SHIFT] = false;
				break;
			case SDLK_LCTRL:
				m_keysPressed[KEY_LEFT_CONTROL] = false;
				break;
			case SDLK_LALT:
				m_keysPressed[KEY_LEFT_ALT] = false;
				break;
			case SDLK_SPACE:
				m_keysPressed[KEY_SPACE] = false;
				break;
			case SDLK_RCTRL:
				m_keysPressed[KEY_RIGHT_CONTROL] = false;
				break;
			default:
				break;
			}
		}
		else if (m_event.type == SDL_WINDOWEVENT)
		{
			if (m_event.window.event == SDL_WINDOWEVENT_CLOSE)
				windowCloseCalled = true;
		}
		
	}

	HandleInputWithEvents();

	//auto stop = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	//std::cout << duration.count() << std::endl;

	profiler->endProfile("Input Update");
}

bool Input::KeyIsPressed(KEYS_PRESSED_LIST key)
{
	return m_keysPressed[key];
}

void Input::setUp()
{
	int index = 0;
	while (index != SIZE_OF_KEYS_PRESSED_ENUM)
	{
		m_keysPressed[index] = 0;
		m_keysPressedPrevious[index] = 0;
		index++;
	}
}

bool Input::windowCloseEventCalled()
{
	return windowCloseCalled;
}

Input::Input()
{
	setUp();
}

Input::~Input()
{

}

void Input::HandleInputWithEvents()
{
	for (int index = 0; index < SIZE_OF_KEYS_PRESSED_ENUM; index++)
	{
		if (m_keysPressed[index] != m_keysPressedPrevious[index])
		{
			Event_Manager->FireEvent(eventType(index), new Event(Event::TYPE_BOOL, m_keysPressed[index]));
		}
		m_keysPressedPrevious[index] = m_keysPressed[index];
	}
}
