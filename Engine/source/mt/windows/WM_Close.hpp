// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "WindowsMessage.hpp"

namespace mt::windows
{
    // Orderly Shutdown
    class WM_Close : public WindowsMessage
    {
        LRESULT execute(const HWND &hwnd, const UINT &msg, const WPARAM &wParam, const LPARAM &lParam);
    };
}