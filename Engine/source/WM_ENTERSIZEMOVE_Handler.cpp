#include "WM_ENTERSIZEMOVE_Handler.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

LRESULT WM_ENTERSIZEMOVE_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& engine = engine::GetEngine();

	engine::SetIsWindowResizing(true);

	engine.GetTimerManager().pause_time();

	return 0;
}

