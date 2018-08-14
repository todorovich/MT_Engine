// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_ExitSizeMove.hpp"

#include "engine.hpp"

LRESULT mt::WM_ExitSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& Engine = Engine::GetEngine();

	Engine::Resize(Engine.GetWindowWidth(), Engine.GetWindowHeight());
	
	Engine::GetTimerManager().Continue();
	
	Engine::SetIsWindowResizing(false);
	
	return 0;
}