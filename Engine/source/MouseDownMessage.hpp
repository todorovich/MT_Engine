#pragma once

#include "InputMessage.hpp"

namespace mt
{
	class MouseDownMessage : public InputMessage
	{
	public:

		MouseDownMessage(WPARAM button_down)
			: InputMessage(static_cast<__int64>(button_down))
		{}

		MouseDownMessage(const MouseDownMessage& other)
			: InputMessage(other)
		{}


		MouseDownMessage(MouseDownMessage&& other)
			: InputMessage(std::move(other))
		{}

		~MouseDownMessage() = default;

		MouseDownMessage& operator=(const MouseDownMessage& other)
		{
			i64 = other.i64;

			return *this;
		}

		MouseDownMessage& operator=(MouseDownMessage&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}

		virtual void Execute()
		{
			_MouseDown(_button_down);
		};

	private:
		WPARAM& _button_down = *reinterpret_cast<WPARAM*>(&i64);
	};
}