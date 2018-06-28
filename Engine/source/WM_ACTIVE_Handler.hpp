#pragma once

#include "../mt/WindowsMessageHandler.hpp"

class WM_CAPTURECHANGED_Handler : public WindowsMessageHandler
{
	LRESULT execute(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
};
