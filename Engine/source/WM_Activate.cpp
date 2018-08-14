// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Activate.hpp"

#include "engine.hpp"

LRESULT mt::WM_Activate::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	if (LOWORD(wParam) == WA_INACTIVE)
	{
		Engine::GetTimerManager().Pause();
	}
	else
	{
		Engine::GetTimerManager().Continue();
	}

	return 0;
}

