// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_LeftMouseButtonDown.hpp"

#include "Engine.hpp"

using namespace mt;

LRESULT WM_LeftMouseButtonDown::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputManager().MouseDown(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}
