#pragma once

//#include "precompiled.hpp"

#include "WindowsMessage.hpp"

namespace mt
{
class WM_LeftMouseButtonDown : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}
