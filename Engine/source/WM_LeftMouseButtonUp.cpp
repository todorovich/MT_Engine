#include "WM_LeftMouseButtonUp.hpp"

#include "Engine.hpp"

LRESULT mt::WM_LeftMouseButtonUp::execute(const HWND & hwnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
{
	Engine::GetInputHandler().MouseUp(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	return 0;
}
