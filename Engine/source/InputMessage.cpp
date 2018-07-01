#include "InputMessage.hpp"

#include "engine.hpp"

using namespace mt;

 void InputMessage::_MouseMove(__int32 x, __int32 y)
{
	Engine::GetInputHandler()._MouseMove(x, y);
}

void InputMessage::_MouseDown(WPARAM btnState)
{
	Engine::GetInputHandler()._MouseDown(btnState);
}

void InputMessage::_MouseUp(WPARAM btnState)
{
	Engine::GetInputHandler()._MouseUp(btnState);
}

void InputMessage::_KeyDown(WPARAM vk_key)
{
	Engine::GetInputHandler()._KeyDown(vk_key);
}

void InputMessage::_KeyUp(WPARAM vk_key)
{
	Engine::GetInputHandler()._KeyUp(vk_key);
}
