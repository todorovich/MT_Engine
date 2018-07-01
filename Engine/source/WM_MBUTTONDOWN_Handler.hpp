#pragma once

#include "WindowsMessage.hpp"
#include "engine.hpp"

namespace mt
{
	class WM_MBUTTONDOWN_Handler : public WindowsMessage
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{
			Engine::GetInputHandler().MouseDown(MK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
			return 0;
		}
	};
}
