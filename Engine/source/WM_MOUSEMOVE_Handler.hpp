#pragma once

#include "WindowsMessageHandler.hpp"
#include "engine.hpp"

namespace mt
{
	class WM_MOUSEMOVE_Handler : public WindowsMessageHandler
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{
			engine::get_input_handler().MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
			return 0; // If an application processes this message, it should return zero.
		}
	};
}
