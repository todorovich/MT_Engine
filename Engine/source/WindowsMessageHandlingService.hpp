#pragma once

#include "precompiled.hpp"

#include "WindowsMessageHandler.hpp"

namespace mt
{
	class WindowsMessageHandlingService
	{
	public:
		WindowsMessageHandlingService();
	
		~WindowsMessageHandlingService();
	
		LRESULT handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
	
		std::map<int, WindowsMessageHandler*> _message_handler_map;
	};
}