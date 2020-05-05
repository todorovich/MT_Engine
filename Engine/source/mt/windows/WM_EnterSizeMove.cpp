// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_EnterSizeMove.hpp"

#include "Engine.hpp"

using namespace mt::windows;

LRESULT WM_EnterSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& Engine = Engine::GetEngine();

	Engine::SetIsWindowResizing(true);

	Engine.GetTimeManager().Pause();

	return 0;
}

