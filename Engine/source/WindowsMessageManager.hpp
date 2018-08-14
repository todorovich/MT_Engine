// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

#include "WindowsMessage.hpp"

namespace mt
{
	class WindowsMessageManager
	{
	public:
		WindowsMessageManager();
	
		~WindowsMessageManager();
	
		LRESULT handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
	
		std::map<int, WindowsMessage*> _message_handler_map;
	};
}