// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

#include "InputMessage.hpp"

namespace mt::input
{
	class IM_MouseDown : public InputMessage
	{
	public:

		IM_MouseDown(WPARAM button_down)
			: InputMessage(static_cast<__int64>(button_down))
		{}

		IM_MouseDown(const IM_MouseDown& other)
			: InputMessage(other)
		{}


		IM_MouseDown(IM_MouseDown&& other)
			: InputMessage(std::move(other))
		{}

		~IM_MouseDown() = default;

		IM_MouseDown& operator=(const IM_MouseDown& other)
		{
			i64 = other.i64;

			return *this;
		}

		IM_MouseDown& operator=(IM_MouseDown&& other)
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