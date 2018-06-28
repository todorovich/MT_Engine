#include "WM_EXITSIZEMOVE_Handler.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

LRESULT WM_EXITSIZEMOVE_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& engine = engine::get_engine();

	engine::resize(engine.get_width(), engine.get_height());
	
	engine::get_game_timer().unpause_time();
	
	engine::set_is_resizing(false);
	
	return 0;
}