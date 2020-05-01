// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Activate.hpp"

#include "mt/Engine.hpp"

LRESULT mt::WM_Activate::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	if (LOWORD(wParam) == WA_INACTIVE)
	{
		Engine::GetTimeManager().Pause();
	}
	else
	{
		Engine::GetTimeManager().Continue();
	}

	return 0;
}

