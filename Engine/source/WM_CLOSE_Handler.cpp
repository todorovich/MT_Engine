#include "WM_CLOSE_Handler.hpp"

#include "engine.hpp"

using namespace mt;

LRESULT WM_CLOSE_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_CLOSE_Handler\n");

	// this is where orderly requests to quit come from

	// Confirm the user wants to close.

	static const bool user_wants_to_close = true;

	if (user_wants_to_close)
	{
		// Pause the game loop
		engine::GetTimerManager().paused_time();

		// Stop the game loop
		engine::Shutdown();

		// Save any state that needs to be saved
		// engine::save(); ?

		// Destroy the window
		DestroyWindow(hwnd);
	}

	return 0;
}
