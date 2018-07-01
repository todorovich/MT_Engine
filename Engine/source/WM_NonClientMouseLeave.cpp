#include "WM_NonClientMouseLeave.hpp"

LRESULT mt::WM_NonClientMouseLeave::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
