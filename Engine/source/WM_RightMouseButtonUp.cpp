// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_RightMouseButtonUp.hpp"

#include "Engine.hpp"

LRESULT mt::WM_RightMouseButtonUp::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputHandler().MouseUp(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}
