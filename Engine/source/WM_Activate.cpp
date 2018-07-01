#include "WM_Activate.hpp"

#include "engine.hpp"

LRESULT mt::WM_Activate::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	if (LOWORD(wParam) == WA_INACTIVE)
	{
		Engine::GetTimerManager().pause_time();
	}
	else
	{
		Engine::GetTimerManager().unpause_time();
	}

	return 0;
}

