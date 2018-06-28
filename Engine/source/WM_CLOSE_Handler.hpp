#pragma once

#include "WindowsMessageHandler.hpp"

// Orderly Shutdown
class WM_CLOSE_Handler : public WindowsMessageHandler
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
