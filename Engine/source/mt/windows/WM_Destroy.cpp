// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Destroy.hpp"

#include "mt/Engine.hpp"

using namespace mt::windows;

// This message is sent when a window is being destroyed. It is sent to the window procedure of 
// the window being destroyed after the window is removed from the screen.
// This message is sent first to the window being destroyed and then to the child windows, if any, 
// as they are destroyed.During the processing of the message, it can be assumed that all child windows still exist.
LRESULT WM_Destroy::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	OutputDebugStringW(L"WM_Destroy\n");

	// Causes quit message to appear which will end the windows message processing thread.
	PostQuitMessage(0);
	
	return 0;
}
