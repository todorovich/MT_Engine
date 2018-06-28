#pragma once

#include "precompiled.hpp"

namespace mt
{
class InputHandler
{
	public:
	InputHandler() = default;
	InputHandler(const InputHandler& other) = delete;
	InputHandler& operator=(const InputHandler& other) = delete;
	~InputHandler() = default;


	void MouseDown(WPARAM btnState, int x, int y);

	void MouseUp(WPARAM btnState, int x, int y);

	void MouseMove(WPARAM btnState, int x, int y);
	
	void KeyDown(WPARAM vk_key, LPARAM flags);

	void KeyUp(WPARAM vk_key, LPARAM flags);

protected:
	POINT	_mouse_position;


};
}