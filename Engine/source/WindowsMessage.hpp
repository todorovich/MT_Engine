// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

class WindowsMessage
{
public:
	virtual ~WindowsMessage() = default;
	
	virtual LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam) = 0;
};




