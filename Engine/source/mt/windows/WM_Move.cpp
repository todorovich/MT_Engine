// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Move.hpp"

using namespace mt::windows;

LRESULT WM_Move::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
