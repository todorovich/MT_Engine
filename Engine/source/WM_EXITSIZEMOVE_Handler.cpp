#include "WM_EXITSIZEMOVE_Handler.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

LRESULT WM_EXITSIZEMOVE_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& Engine = Engine::GetEngine();

	Engine::Resize(Engine.GetWindowWidth(), Engine.GetWindowHeight());
	
	Engine::GetTimerManager().unpause_time();
	
	Engine::SetIsWindowResizing(false);
	
	return 0;
}