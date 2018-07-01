#pragma once

#include "precompiled.hpp"

#include "InputMessage.hpp"

namespace mt
{
	class MouseUpMessage : public InputMessage
	{
	public:

		MouseUpMessage(WPARAM button_down)
			: InputMessage(static_cast<__int64>(button_down))
		{}

		MouseUpMessage(const MouseUpMessage& other)
			: InputMessage(other)
		{}

		MouseUpMessage(MouseUpMessage&& other)
			: InputMessage(std::move(other))
		{}

		~MouseUpMessage() = default;

		MouseUpMessage& operator=(const MouseUpMessage& other)
		{
			i64 = other.i64;

			return *this;
		}

		MouseUpMessage& operator=(MouseUpMessage&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}

		virtual void Execute()
		{
			_MouseUp(_button_up);
		};

	private:
		WPARAM & _button_up = *reinterpret_cast<WPARAM*>(&i64);
	};
}