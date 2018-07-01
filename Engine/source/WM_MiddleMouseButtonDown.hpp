#pragma once

#include "WindowsMessage.hpp"

namespace mt
{
class WM_MiddleMouseButtonDown : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}