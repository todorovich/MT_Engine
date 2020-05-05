// Copyright 2018 Micho Todorovich, all rights reserved.
#include "InputManager.hpp"

#include "Engine.hpp"

#include "IM_MouseMove.hpp"
#include "IM_MouseDown.hpp"
#include "IM_MouseUp.hpp"
#include "IM_KeyDown.hpp"
#include "IM_KeyUp.hpp"

using namespace mt::input;

void InputManager::ProcessInput() 
{
	auto size = _input_queue.size();
	for (auto x = 0; x < size; x++)
	{
		InputMessage* input_message = _input_queue.front();
		_input_queue.pop();
		input_message->Execute();
		_message_pool.ReleaseMemory(input_message);
	}
}

void InputManager::MouseMove(WPARAM button_state, int x, int y)
{
	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseMove(x, y)));
}

void InputManager::MouseDown(WPARAM button_state, int x, int y)
{	
	SetCapture(Engine::GetMainWindowHandle());

	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseDown(button_state)));
}

void InputManager::MouseUp(WPARAM button_state, int x, int y)
{
	ReleaseCapture();

	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_MouseUp(button_state)));
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
void InputManager::KeyDown(WPARAM vk_key, LPARAM flags)
{
	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_KeyDown(vk_key)));
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646281(v=vs.85).aspx
void InputManager::KeyUp(WPARAM vk_key, LPARAM flags)
{
	_input_queue.push(reinterpret_cast<InputMessage*>(new (_message_pool.GetMemory()) IM_KeyUp(vk_key)));
}

void InputManager::_MouseMove(__int32 x, __int32 y)
{
	// Left mouse button is being held
	if (_held_keys_and_buttons.find(MK_LBUTTON) != _held_keys_and_buttons.end())
	{
		auto& camera = Engine::GetCurrentCamera();

		// Make each pixel correspond to 1/10th of a degree.
		float dx = XMConvertToRadians(0.1f*static_cast<float>(x - _mouse_position.x));
		float dy = XMConvertToRadians(0.1f*static_cast<float>(y - _mouse_position.y));

		camera.Pitch(dy);
		camera.RotateY(dx);
	}

	_mouse_position.x = x;
	_mouse_position.y = y;
}

void InputManager::_MouseDown(WPARAM btnState)
{
	_held_keys_and_buttons.insert(btnState);
}

void InputManager::_MouseUp(WPARAM btnState)
{
	_held_keys_and_buttons.erase(btnState);
}

void InputManager::_KeyDown(WPARAM vk_key)
{

}

void InputManager::_KeyUp(WPARAM vk_key)
{
	if (vk_key == VK_ESCAPE)
	{
		PostMessage(Engine::GetMainWindowHandle(), WM_CLOSE, 0, 0);
	}

	// P Key
	else if (vk_key == 0x50)
	{
		if (Engine::GetTimeManager().IsUpdatePaused())
		{
			Engine::GetTimeManager().Continue();
		}
		else
		{
			Engine::GetTimeManager().Pause();
		}
	}
}