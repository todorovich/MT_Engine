// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_NonClientCalcSize.hpp"

using namespace mt::windows;

LRESULT WM_NonClientCalcSize::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
