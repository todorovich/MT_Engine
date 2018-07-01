#include "engine.hpp"

#include "WM_ACTIVATE_Handler.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

LRESULT WM_ACTIVATE_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& _time_manager = engine::GetEngine().GetTimerManager();

	if (LOWORD(wParam) == WA_INACTIVE)
	{
		_time_manager.pause_time();
	}
	else
	{
		_time_manager.unpause_time();
	}

	return 0;
}

