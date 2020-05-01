// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "WindowsMessage.hpp"
#include "mt/Engine.hpp"

namespace mt
{
class WM_MouseMove : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}
