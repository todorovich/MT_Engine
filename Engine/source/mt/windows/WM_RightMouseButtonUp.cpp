// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_RightMouseButtonUp.hpp"

#include "Engine.hpp"

using namespace mt::windows;

LRESULT WM_RightMouseButtonUp::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputManager().MouseUp(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}
