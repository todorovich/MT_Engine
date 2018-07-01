#include "WM_EnterSizeMove.hpp"

#include "engine.hpp"

LRESULT mt::WM_EnterSizeMove::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& Engine = Engine::GetEngine();

	Engine::SetIsWindowResizing(true);

	Engine.GetTimerManager().pause_time();

	return 0;
}

