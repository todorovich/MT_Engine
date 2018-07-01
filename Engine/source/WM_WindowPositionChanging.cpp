#include "WM_WindowPositionChanging.hpp"

LRESULT mt::WM_WindowPositionChanging::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
