#pragma once

#include "WindowsMessageHandler.hpp"

class WM_GETMINMAXINFO_Handler : public WindowsMessageHandler
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
	{
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;

		return 0;
	}
};
