#pragma once

#include "WindowsMessage.hpp"

// Orderly Shutdown
class WM_CLOSE_Handler : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
