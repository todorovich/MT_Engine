#pragma once



#include "precompiled.hpp"

#include "InputMessage.hpp"

namespace mt
{
class InputHandler
{
public:
	InputHandler()
	{
		_pool_start = new InputMessage[512];
		_pool_end = &_pool_start[512];

		for (short i = 0; i < 512; i++)
		{
			_free_pool_slots.insert(i);
		}
	}

	InputHandler(const InputHandler& other) = delete;
	InputHandler& operator=(const InputHandler& other) = delete;
	~InputHandler()
	{
		delete[] _pool_start;
		_pool_start = nullptr;
		_pool_end = nullptr;
	}

	void ProcessInput();

	void MouseMove(WPARAM btnState, int x, int y);

	void MouseDown(WPARAM btnState, int x, int y);

	void MouseUp(WPARAM btnState, int x, int y);
		
	void KeyDown(WPARAM vk_key, LPARAM flags);

	void KeyUp(WPARAM vk_key, LPARAM flags);

	friend ::mt::InputMessage;
	//friend class MouseMove;


protected:

	void _MouseMove(__int32 x, __int32 y);

	void _MouseDown(WPARAM btnState);

	void _MouseUp(WPARAM btnState);

	void _KeyDown(WPARAM vk_key);

	void _KeyUp(WPARAM vk_key);

private:

	InputMessage* Allocate();

	void Deallocate(InputMessage* ptr);

	std::queue<InputMessage*> _input_queue;
	
	std::mutex _input_queue_lock;
	
	POINT	_mouse_position;
	
	std::set<WPARAM> _held_keys_and_buttons;

	std::set<short> _free_pool_slots;

	std::set<short> _used_pool_slots;
	
	InputMessage* _pool_start;
	InputMessage* _pool_end;

	int _pool_index = 0;
};
}