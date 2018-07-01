#include "WM_KEYUP_Handler.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace mt
{
	LRESULT WM_KEYUP_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
	{
		//OutputDebugStringW(L"WM_KEY_UP_Handler\n");
	
		Engine::GetInputHandler().KeyUp(wParam, lParam);
	
		// An application should return zero if it processes this message.
		return 0;
	}
}

