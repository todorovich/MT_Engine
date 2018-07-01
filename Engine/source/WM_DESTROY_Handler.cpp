#include "WM_DESTROY_Handler.hpp"

#include "engine.hpp"

using namespace mt;

LRESULT WM_DESTROY_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_DESTROY_Handler\n");

	// this should cause the Engine to quit immediately
	Engine::Destroy();

	// Causes message handling thread to end
	PostQuitMessage(0);
	
	return 0;
}
