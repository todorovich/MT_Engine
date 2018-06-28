#pragma once

#include "WindowsMessageHandler.hpp"
#include "engine.hpp"

namespace mt
{
	// This quit does not give a fuck. Must quit now.
	class WM_QUIT_Handler : public WindowsMessageHandler
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{	
			// Put in code to ask if you are sure?
			
			return 0;
		}
	};
}
