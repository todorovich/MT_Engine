#pragma once

#include "WindowsMessage.hpp"

namespace mt
{
class WM_LeftMouseButtonUp : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}