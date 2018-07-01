#pragma once

#include "WindowsMessage.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace mt
{
	class WM_SIZING_Handler : public WindowsMessage
	{
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	};
}
