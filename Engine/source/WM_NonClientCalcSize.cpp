// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_NonClientCalcSize.hpp"

LRESULT mt::WM_NonClientCalcSize::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
