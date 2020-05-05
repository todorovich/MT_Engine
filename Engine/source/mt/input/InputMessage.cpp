// Copyright 2018 Micho Todorovich, all rights reserved.

#include "InputMessage.hpp"

#include "Engine.hpp"

using namespace mt::input;

 void InputMessage::_MouseMove(__int32 x, __int32 y)
{
	Engine::GetInputManager()._MouseMove(x, y);
}

void InputMessage::_MouseDown(WPARAM btnState)
{
	Engine::GetInputManager()._MouseDown(btnState);
}

void InputMessage::_MouseUp(WPARAM btnState)
{
	Engine::GetInputManager()._MouseUp(btnState);
}

void InputMessage::_KeyDown(WPARAM vk_key)
{
	Engine::GetInputManager()._KeyDown(vk_key);
}

void InputMessage::_KeyUp(WPARAM vk_key)
{
	Engine::GetInputManager()._KeyUp(vk_key);
}
