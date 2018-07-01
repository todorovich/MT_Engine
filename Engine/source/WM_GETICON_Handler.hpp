#pragma once

#include "WindowsMessage.hpp"

class WM_GETICON_Handler : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
};
