#pragma once

#include "WindowsMessageHandler.hpp"
#include "engine.hpp"

namespace mt
{
	class WM_INPUT_Handler : public WindowsMessageHandler
	{
		unsigned buffer_size = 40;
	
		char* buffer = nullptr;
	
		UINT cbSizeT = 64;
	
		PRAWINPUT pRawInput = nullptr;
	
	public:
		WM_INPUT_Handler();

		~WM_INPUT_Handler();
	
		LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
	};
}
