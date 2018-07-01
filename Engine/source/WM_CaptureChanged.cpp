#include "WM_CaptureChanged.hpp"

LRESULT mt::WM_CaptureChanged::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
