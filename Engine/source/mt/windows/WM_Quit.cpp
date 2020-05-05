// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Quit.hpp"

#include "Engine.hpp"

using namespace mt::windows;

LRESULT WM_Quit::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	// This should be the final windows message received as this causes the windows message loop to stop
	// and the main engine tick thread to be joined

	// Command doesn't actually ever get called.

	//OutputDebugStringW(L"WM_Quit\n");

	return 0;
}
