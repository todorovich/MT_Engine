// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WindowsMessageManager.hpp"

// Various Messages from windows
#include "WM_Activate.hpp"
#include "WM_CaptureChanged.hpp"
#include "WM_Close.hpp"
#include "WM_Destroy.hpp"
#include "WM_EnterSizeMove.hpp"
#include "WM_EraseBackground.hpp"
#include "WM_ExitSizeMove.hpp"
#include "WM_GetIcon.hpp"
#include "WM_GetMinMaxInfo.hpp"
#include "WM_Input.hpp"
#include "WM_KeyUp.hpp"
#include "WM_LeftMouseButtonDown.hpp"
#include "WM_LeftMouseButtonUp.hpp"
#include "WM_MiddleMouseButtonDown.hpp"
#include "WM_MiddleMouseButtonUp.hpp"
#include "WM_MenuChar.hpp"
#include "WM_MouseMove.hpp"
#include "WM_Move.hpp"
#include "WM_Moving.hpp"
#include "WM_NonClientCalcSize.hpp"
#include "WM_NonClientMouseLeave.hpp"
#include "WM_NonClientPaint.hpp"
#include "WM_Paint.hpp"
#include "WM_Quit.hpp"
#include "WM_RightMouseButtonDown.hpp"
#include "WM_RightMouseButtonUp.hpp"
#include "WM_SetText.hpp"
#include "WM_Size.hpp"
#include "WM_Sizing.hpp"
#include "WM_Timer.hpp"
#include "WM_WindowPositionChanging.hpp"

using namespace mt;

WindowsMessageManager::WindowsMessageManager()
{
	// Add all the message handlers to the message handler map
	_message_handler_map.insert(std::make_pair(WM_ACTIVATE, new WM_Activate()));
	_message_handler_map.insert(std::make_pair(WM_CAPTURECHANGED, new WM_CaptureChanged()));
	_message_handler_map.insert(std::make_pair(WM_CLOSE, new WM_Close()));
	_message_handler_map.insert(std::make_pair(WM_DESTROY, new WM_Destroy()));
	_message_handler_map.insert(std::make_pair(WM_ENTERSIZEMOVE, new WM_EnterSizeMove()));
	_message_handler_map.insert(std::make_pair(WM_ERASEBKGND, new WM_EraseBackground()));
	_message_handler_map.insert(std::make_pair(WM_EXITSIZEMOVE, new WM_ExitSizeMove()));
	_message_handler_map.insert(std::make_pair(WM_GETICON, new WM_GetIcon()));
	_message_handler_map.insert(std::make_pair(WM_GETMINMAXINFO, new WM_GetMinMaxInfo()));
	_message_handler_map.insert(std::make_pair(WM_INPUT, new WM_Input()));
	_message_handler_map.insert(std::make_pair(WM_KEYUP, new WM_KeyUp()));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONDOWN, new WM_LeftMouseButtonDown()));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONUP, new WM_LeftMouseButtonUp()));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONDOWN, new WM_MiddleMouseButtonDown()));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONUP, new WM_MiddleMouseButtonUp()));
	_message_handler_map.insert(std::make_pair(WM_MENUCHAR, new WM_MenuChar()));
	_message_handler_map.insert(std::make_pair(WM_MOUSEMOVE, new WM_MouseMove()));
	_message_handler_map.insert(std::make_pair(WM_MOVE, new WM_Move()));
	_message_handler_map.insert(std::make_pair(WM_MOVING, new WM_Moving()));
	_message_handler_map.insert(std::make_pair(WM_NCCALCSIZE, new WM_NonClientCalcSize()));
	_message_handler_map.insert(std::make_pair(WM_NCMOUSELEAVE, new WM_NonClientMouseLeave()));
	_message_handler_map.insert(std::make_pair(WM_NCPAINT, new WM_NonClientPaint()));
	_message_handler_map.insert(std::make_pair(WM_PAINT, new WM_Paint()));
	_message_handler_map.insert(std::make_pair(WM_QUIT, new WM_Quit()));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONDOWN, new WM_RightMouseButtonDown()));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONUP, new WM_RightMouseButtonUp()));
	_message_handler_map.insert(std::make_pair(WM_SETTEXT, new WM_SetText()));
	_message_handler_map.insert(std::make_pair(WM_SIZE, new WM_Size()));
	_message_handler_map.insert(std::make_pair(WM_SIZING, new WM_Sizing()));
	_message_handler_map.insert(std::make_pair(WM_TIMER, new WM_Timer()));
	_message_handler_map.insert(std::make_pair(WM_WINDOWPOSCHANGING, new WM_WindowPositionChanging()));
}

WindowsMessageManager::~WindowsMessageManager()
{
	// clean up all those message handlers
	for (auto& pair : _message_handler_map)
	{
		delete pair.second;
	}
}

LRESULT WindowsMessageManager::handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	auto& it = _message_handler_map.find(msg);

	if (it != _message_handler_map.end())
	{
		auto result = (*it).second->execute(hwnd, msg, wParam, lParam);

		return result;
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

