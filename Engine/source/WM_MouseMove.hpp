#pragma once

#include "WindowsMessage.hpp"
#include "engine.hpp"

namespace mt
{
class WM_MouseMove : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}
