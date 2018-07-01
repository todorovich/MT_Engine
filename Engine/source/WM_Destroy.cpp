#include "WM_Destroy.hpp"

#include "engine.hpp"

LRESULT mt::WM_Destroy::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_Destroy\n");

	// this should cause the Engine to quit immediately
	Engine::Destroy();

	// Causes message handling thread to end
	PostQuitMessage(0);
	
	return 0;
}
