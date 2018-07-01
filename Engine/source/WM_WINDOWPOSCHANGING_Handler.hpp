#pragma once

#include "WindowsMessage.hpp"

namespace mt
{
	class WM_WINDOWPOSCHANGING_Handler : public WindowsMessage
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	};
}
