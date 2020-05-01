// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Sizing.hpp"

#include "mt/Engine.hpp"

LRESULT mt::WM_Sizing::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
