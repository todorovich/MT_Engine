#pragma once

#include "WindowsMessageHandler.hpp"
#include "engine.hpp"

namespace mt
{
	class WM_RBUTTONUP_Handler : public WindowsMessageHandler
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{
			engine::get_input_handler().MouseUp(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
			return 0;
		}
	};
}
