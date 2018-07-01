#pragma once

#include "precompiled.hpp"

#include "InputMessage.hpp"

namespace mt
{
	class KeyDownMessage : public InputMessage
	{
	public:

		KeyDownMessage(WPARAM button_down)
			: InputMessage(static_cast<__int64>(button_down))
		{}

		KeyDownMessage(const KeyDownMessage& other)
			: InputMessage(other)
		{}

		KeyDownMessage(KeyDownMessage&& other)
			: InputMessage(std::move(other))
		{}

		~KeyDownMessage() = default;

		KeyDownMessage& operator=(const KeyDownMessage& other)
		{
			i64 = other.i64;

			return *this;
		}

		KeyDownMessage& operator=(KeyDownMessage&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}

		virtual void Execute()
		{
			_KeyDown(_button_down);
		};

	private:
		WPARAM & _button_down = *reinterpret_cast<WPARAM*>(&i64);
	};
}