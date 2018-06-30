#include "WM_SIZE_Handler.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

LRESULT WM_SIZE_Handler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	// Save the new client area dimensions.
	auto _client_width = LOWORD(lParam);
	auto _client_height = HIWORD(lParam);

	engine::get_engine().set_window_dimensions(_client_width, _client_height);

	if (engine::get_renderer().get_is_initialized())
	{
		if (wParam == SIZE_MINIMIZED)
		{
			engine::get_game_timer().pause_time();
			engine::set_is_minimized(true);
			engine::set_is_maximized(false);
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			engine::get_game_timer().unpause_time();
			engine::set_is_minimized(false);
			engine::set_is_maximized(true);
			engine::resize(_client_width, _client_height);
		}
		else if (wParam == SIZE_RESTORED)
		{
			// Restoring from minimized state?
			if (engine::get_engine().get_is_minimized())
			{
				engine::get_game_timer().unpause_time();
				engine::set_is_minimized(false);
				engine::resize(_client_width, _client_height);
			}

			// Restoring from maximized state?
			else if (engine::get_engine().get_is_maximized())
			{
				engine::get_game_timer().unpause_time();
				engine::set_is_minimized(false);
				engine::resize(_client_width, _client_height);
			}

			else if (engine::get_engine().get_is_resizing())
			{
				// If user is dragging the resize bars, we do not resize 
				// the buffers here because as the user continuously 
				// drags the resize bars, a stream of WM_SIZE messages are
				// sent to the window, and it would be pointless (and slow)
				// to resize for each WM_SIZE message received from dragging
				// the resize bars.  So instead, we reset after the user is 
				// done resizing the window and releases the resize bars, which 
				// sends a WM_EXITSIZEMOVE message.


			}
			else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
			{
				engine::resize(_client_width, _client_height);
			}
		}
	}

	return 0;
}

