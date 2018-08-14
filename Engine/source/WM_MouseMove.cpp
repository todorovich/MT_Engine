// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_MouseMove.hpp"

LRESULT mt::WM_MouseMove::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputHandler().MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0; // If an application processes this message, it should return zero.
}
