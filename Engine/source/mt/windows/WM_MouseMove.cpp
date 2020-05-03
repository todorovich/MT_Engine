// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_MouseMove.hpp"

using namespace mt::windows;

LRESULT WM_MouseMove::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputManager().MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0; // If an application processes this message, it should return zero.
}
