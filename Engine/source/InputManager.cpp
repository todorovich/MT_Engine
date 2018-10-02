// Copyright 2018 Micho Todorovich, all rights reserved.
#include "InputManager.hpp"

#include "engine.hpp"

#include "IM_MouseMove.hpp"
#include "IM_MouseDown.hpp"
#include "IM_MouseUp.hpp"
#include "IM_KeyDown.hpp"
#include "IM_KeyUp.hpp"

//#ifdef _DEBUG
//#define  _CRTDBG_MAP_ALLOC
//#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

using namespace mt;

void InputManager::ProcessInput() 
{
	auto size = _input_queue.size();

	if (size != 0)
	{
		//while (_input_queue_lock.try_lock() == false);

		for (auto x = 0; x < size; x++)
		{
			if (!_input_queue.empty())
			{
				mt::InputMessage* input_message = _input_queue.front();
				_input_queue.pop();
				input_message->Execute();
				delete input_message;
			}
		}
	
		//_input_queue_lock.unlock();
	}
}

void InputManager::MouseMove(WPARAM btnState, int x, int y)
{
	if (x != _mouse_position.x && y != _mouse_position.y)
	{
		//while (_input_queue_lock.try_lock() == false);

		//_input_queue.push(reinterpret_cast<InputMessage*>(new (&_pool_start[_pool_index]) IM_MouseMove(x, y)));

		//_pool_index = (_pool_index + 1) % 512;

		//_input_queue_lock.unlock();

		_MouseMove(x, y);
	}
}

void InputManager::MouseDown(WPARAM btnState, int x, int y)
{	
	SetCapture(Engine::GetMainWindowHandle());

	//while (_input_queue_lock.try_lock() == false);
	
	if (x != _mouse_position.x && y != _mouse_position.y)
	{	
		//_input_queue.push(reinterpret_cast<InputMessage*>(new (&_pool_start[_pool_index]) IM_MouseMove(x, y)));

		//_pool_index = (_pool_index + 1) % 512;

		_MouseMove(x, y);
	}

	//_input_queue.push(reinterpret_cast<InputMessage*>(new (&_pool_start[_pool_index]) IM_MouseDown(btnState)));
	
	//_pool_index = (_pool_index + 1) % 512;

	_MouseDown(btnState);

	//_input_queue_lock.unlock();	
}

void InputManager::MouseUp(WPARAM btnState, int x, int y)
{
	//while (_input_queue_lock.try_lock() == false);
	
	if (x != _mouse_position.x && y != _mouse_position.y)
	{
		//_input_queue.push(reinterpret_cast<InputMessage*>(new IM_MouseMove(x, y)));
		_MouseMove(x, y);
	}

	//_input_queue.push(reinterpret_cast<InputMessage*>(new IM_MouseUp(btnState)));

	_MouseUp(btnState);

	//_input_queue_lock.unlock();

	ReleaseCapture();
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
void InputManager::KeyDown(WPARAM vk_key, LPARAM flags)
{
	//while (_input_queue_lock.try_lock() == false);

	//_input_queue.push(reinterpret_cast<InputMessage*>(new IM_KeyDown(vk_key)));

	_KeyDown(vk_key);

	//_input_queue_lock.unlock();
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646281(v=vs.85).aspx
void InputManager::KeyUp(WPARAM vk_key, LPARAM flags)
{
	//while (_input_queue_lock.try_lock() == false);

	//_input_queue.push(reinterpret_cast<InputMessage*>(new IM_KeyUp(vk_key)));

	_KeyUp(vk_key);

	//_input_queue_lock.unlock();
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
		if (Engine::GetTimerManager().IsUpdatePaused())
		{
			Engine::GetTimerManager().Continue();
		}
		else
		{
			Engine::GetTimerManager().Pause();
		}
	}
}

InputMessage* mt::InputManager::Allocate()
{
	if (_free_pool_slots.empty() == false)
	{
		auto& next_available_itr = _free_pool_slots.begin();

		short next_available = *next_available_itr;

		_free_pool_slots.erase(next_available_itr);

		_used_pool_slots.insert(next_available);

		return &_pool_start[next_available];
	}
	else
	{
		return new InputMessage();
	}

	return nullptr;
}

void mt::InputManager::Deallocate(InputMessage* ptr)
{
	if (ptr >= _pool_start && ptr <= _pool_end)
	{
		short _pool_slot = static_cast<short>(static_cast<__int64>(_pool_end - ptr) % sizeof(InputMessage));

		ptr->~InputMessage();

		_used_pool_slots.erase(_pool_slot);

		_free_pool_slots.insert(_pool_slot);
	}
	else
	{
		delete ptr;
	}
}
