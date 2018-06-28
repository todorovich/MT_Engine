#pragma once

#include "WindowsMessageHandler.hpp"

class WM_NCMOUSELEAVE_Handler : public WindowsMessageHandler
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
};
