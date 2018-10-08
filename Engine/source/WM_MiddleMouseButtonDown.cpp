// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_MiddleMouseButtonDown.hpp"

#include "Engine.hpp"

LRESULT mt::WM_MiddleMouseButtonDown::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputManager().MouseDown(MK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}
