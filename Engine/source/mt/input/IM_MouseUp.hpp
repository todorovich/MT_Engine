// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

#include "InputMessage.hpp"

namespace mt::input
{
	class IM_MouseUp : public InputMessage
	{
	public:

		IM_MouseUp(WPARAM button_down)
			: InputMessage(static_cast<__int64>(button_down))
		{}

		IM_MouseUp(const IM_MouseUp& other)
			: InputMessage(other)
		{}

		IM_MouseUp(IM_MouseUp&& other)
			: InputMessage(std::move(other))
		{}

		~IM_MouseUp() = default;

		IM_MouseUp& operator=(const IM_MouseUp& other)
		{
			i64 = other.i64;

			return *this;
		}

		IM_MouseUp& operator=(IM_MouseUp&& other)
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