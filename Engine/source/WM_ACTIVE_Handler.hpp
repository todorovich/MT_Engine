#pragma once

#include "WindowsMessage.hpp"

class WM_CAPTURECHANGED_Handler : public WindowsMessage
{
	LRESULT execute(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
};
