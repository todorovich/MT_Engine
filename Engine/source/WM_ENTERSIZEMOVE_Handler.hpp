#pragma once

#include "WindowsMessage.hpp"

class WM_ENTERSIZEMOVE_Handler : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
