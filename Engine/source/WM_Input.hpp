// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "WindowsMessage.hpp"

namespace mt
{
class WM_Input : public WindowsMessage
{
	unsigned buffer_size = 40;

	char* buffer = nullptr;

	UINT cbSizeT = 64;

	PRAWINPUT pRawInput = nullptr;

public:
	WM_Input();

	~WM_Input();

	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}
