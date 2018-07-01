#include "WM_NonClientPaint.hpp"

LRESULT mt::WM_NonClientPaint::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
