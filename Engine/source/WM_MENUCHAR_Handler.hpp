#pragma once

#include "WindowsMessage.hpp"

namespace mt
{
	class WM_MENUCHAR_Handler : public WindowsMessage
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{
			return MAKELRESULT(0, MNC_CLOSE);
		}
	};
}
