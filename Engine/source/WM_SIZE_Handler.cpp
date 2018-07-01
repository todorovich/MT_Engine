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
	auto _window_width = LOWORD(lParam);
	auto _window_height = HIWORD(lParam);

	Engine::GetEngine().SetWindowDimensions(_window_width, _window_height);

	if (Engine::GetRenderer().get_is_initialized())
	{
		if (wParam == SIZE_MINIMIZED)
		{
			Engine::GetTimerManager().pause_time();
			Engine::SetIsWindowMinimized(true);
			Engine::SetIsWindowMaximized(false);
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			Engine::GetTimerManager().unpause_time();
			Engine::SetIsWindowMinimized(false);
			Engine::SetIsWindowMaximized(true);
			Engine::Resize(_window_width, _window_height);
		}
		else if (wParam == SIZE_RESTORED)
		{
			// Restoring from minimized state?
			if (Engine::GetEngine().IsWindowMinimized())
			{
				Engine::GetTimerManager().unpause_time();
				Engine::SetIsWindowMinimized(false);
				Engine::Resize(_window_width, _window_height);
			}

			// Restoring from maximized state?
			else if (Engine::GetEngine().IsWindowMaximized())
			{
				Engine::GetTimerManager().unpause_time();
				Engine::SetIsWindowMinimized(false);
				Engine::Resize(_window_width, _window_height);
			}

			else if (Engine::GetEngine().IsWindowResizing())
			{
				// If user is dragging the Resize bars, we do not Resize 
				// the buffers here because as the user continuously 
				// drags the Resize bars, a stream of WM_SIZE messages are
				// sent to the window, and it would be pointless (and slow)
				// to Resize for each WM_SIZE message received from dragging
				// the Resize bars.  So instead, we reset after the user is 
				// done resizing the window and releases the Resize bars, which 
				// sends a WM_EXITSIZEMOVE message.


			}
			else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
			{
				Engine::Resize(_window_width, _window_height);
			}
		}
	}

	return 0;
}

