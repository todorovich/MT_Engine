// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Close.hpp"

#include "Engine.hpp"

using namespace mt::windows;

LRESULT WM_Close::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_Close\n");

	// this is where orderly requests to quit come from

	// Confirm the user wants to close.

	static const bool user_wants_to_close = true;

	if (user_wants_to_close)
	{
		// Shutdown the Game
		Engine::Shutdown();
	}

	return 0;
}
