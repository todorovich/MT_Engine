#pragma once

#include "precompiled.hpp"

namespace mt
{
class InputHandler
{
	public:
	InputHandler() = default;

	void on_mouse_down(WPARAM btnState, int x, int y);

	void on_mouse_up(WPARAM btnState, int x, int y);

	void on_mouse_move(WPARAM btnState, int x, int y);
	
	void on_key_down(WPARAM vk_key, LPARAM flags);

	void on_key_up(WPARAM vk_key, LPARAM flags);

protected:
	POINT	_mouse_position;


};
}