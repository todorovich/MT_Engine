#pragma once

#include <windows.h>
#include <WindowsX.h>

class DirectXRenderer;

class WindowsMessageHandler
{
public:
	virtual ~WindowsMessageHandler() = default;
	
	virtual LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam) = 0;
};




