#pragma once

#include <windows.h>
#include <WindowsX.h>

class DirectXRenderer;

class WindowsMessage
{
public:
	virtual ~WindowsMessage() = default;
	
	virtual LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam) = 0;
};




