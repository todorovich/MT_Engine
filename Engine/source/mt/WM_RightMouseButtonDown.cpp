// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_RightMouseButtonDown.hpp"

#include "mt/Engine.hpp"

LRESULT mt::WM_RightMouseButtonDown::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputManager().MouseDown(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}
