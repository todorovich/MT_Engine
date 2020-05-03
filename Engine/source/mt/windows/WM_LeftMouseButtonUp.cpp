// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_LeftMouseButtonUp.hpp"

#include "mt/Engine.hpp"

using namespace mt::windows;

LRESULT WM_LeftMouseButtonUp::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputManager().MouseUp(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}
