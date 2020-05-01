// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "WindowsMessage.hpp"

namespace mt
{
// This quit does not give a fuck. Must quit now.
class WM_Quit : public WindowsMessage
{
	LRESULT execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);
};
}
