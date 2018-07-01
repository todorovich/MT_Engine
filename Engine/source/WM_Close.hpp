#pragma once

#include "WindowsMessage.hpp"

namespace mt
{
// Orderly Shutdown
class WM_Close : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}