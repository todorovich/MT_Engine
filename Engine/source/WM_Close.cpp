#include "WM_Close.hpp"

#include "engine.hpp"

LRESULT mt::WM_Close::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	//OutputDebugStringW(L"WM_Close\n");

	// this is where orderly requests to quit come from

	// Confirm the user wants to close.

	static const bool user_wants_to_close = true;

	if (user_wants_to_close)
	{
		// Pause the game loop
		Engine::GetTimerManager().paused_time();

		// Stop the game loop
		Engine::Shutdown();

		// Save any state that needs to be saved
		// Engine::save(); ?

		// Destroy the window
		DestroyWindow(hwnd);
	}

	return 0;
}
