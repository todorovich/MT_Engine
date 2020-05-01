// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_KeyUp.hpp"

#include "mt/Engine.hpp"

LRESULT mt::WM_KeyUp::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_KEY_UP_Handler\n");

	Engine::GetInputManager().KeyUp(wParam, lParam);

	// An application should return zero if it processes this message.
	return 0;
}

