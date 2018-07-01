#pragma once

#include "precompiled.hpp"

#include "InputMessage.hpp"

namespace mt
{
	class KeyUpMessage : public InputMessage
	{
	public:

		KeyUpMessage(WPARAM button_up)
			: InputMessage(static_cast<__int64>(button_up))
		{}

		KeyUpMessage(const KeyUpMessage& other)
			: InputMessage(other)
		{}

		KeyUpMessage(KeyUpMessage&& other)
			: InputMessage(std::move(other))
		{}

		~KeyUpMessage() = default;

		KeyUpMessage& operator=(const KeyUpMessage& other)
		{
			i64 = other.i64;

			return *this;
		}

		KeyUpMessage& operator=(KeyUpMessage&& other)
		{
			i64 = std::move(other.i64);

			return *this;
		}

		virtual void Execute()
		{
			_KeyUp(_button_up);
		};

	private:
		WPARAM & _button_up = *reinterpret_cast<WPARAM*>(&i64);
	};
}