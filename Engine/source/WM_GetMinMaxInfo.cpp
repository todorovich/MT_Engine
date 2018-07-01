#include "WM_GetMinMaxInfo.hpp"

LRESULT mt::WM_GetMinMaxInfo::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
	((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;

	return 0;
}
