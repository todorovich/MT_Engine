#pragma once

#include "WindowsMessage.hpp"
#include "engine.hpp"

namespace mt
{
	class WM_MOUSEMOVE_Handler : public WindowsMessage
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{
			Engine::GetInputHandler().MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
			return 0; // If an application processes this message, it should return zero.
		}
	};
}
