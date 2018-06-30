#pragma once

#include "InputMessage.hpp"

namespace mt
{
	class MouseMoveMessage : public InputMessage
	{
	public:

		MouseMoveMessage(__int32 x = 0, __int32 y = 0)
			: InputMessage(x , y)
		{}

		MouseMoveMessage(const MouseMoveMessage& other)
			: InputMessage(other)
		{}

		MouseMoveMessage(MouseMoveMessage&& other)
			: InputMessage(std::move(other))
		{}

		~MouseMoveMessage() = default;

		MouseMoveMessage& operator=(const MouseMoveMessage& other)
		{
			i64 = other.i64;
			
			return *this;
		}

		MouseMoveMessage& operator=(MouseMoveMessage&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}
		
		virtual void Execute() 
		{
			InputMessage::_MouseMove(i32[0], i32[1]);
		};

	private:
		__int32& _x = i32[0];
		__int32& _y = i32[1];
	};
}