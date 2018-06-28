#include "engine.hpp"

#include "WM_ACTIVATE_Handler.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

LRESULT WM_ACTIVATE_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto time = engine::get_engine().get_game_timer();

	if (LOWORD(wParam) == WA_INACTIVE)
	{
		time.pause_time();
	}
	else
	{
		time.unpause_time();
	}

	return 0;
}

