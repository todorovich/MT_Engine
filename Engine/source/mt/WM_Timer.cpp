// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Timer.hpp"

using namespace mt;

LRESULT mt::WM_Timer::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


