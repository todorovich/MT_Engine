#include "WindowsMessageHandlingService.hpp"

// Various Messages from windows
#include "WM_ACTIVATE_Handler.hpp"
#include "WM_CAPTURECHANGED_Handler.hpp"
#include "WM_CLOSE_Handler.hpp"
#include "WM_DESTROY_Handler.hpp"
#include "WM_ENTERSIZEMOVE_Handler.hpp"
#include "WM_ERASEBKGND_Handler.hpp"
#include "WM_EXITSIZEMOVE_Handler.hpp"
#include "WM_GETICON_Handler.hpp"
#include "WM_GETMINMAXINFO_Handler.hpp"
#include "WM_INPUT_Handler.hpp"
#include "WM_KEYUP_Handler.hpp"
#include "WM_LBUTTONDOWN_Handler.hpp"
#include "WM_LBUTTONUP_Handler.hpp"
#include "WM_MBUTTONDOWN_Handler.hpp"
#include "WM_MBUTTONUP_Handler.hpp"
#include "WM_MENUCHAR_Handler.hpp"
#include "WM_MOUSEMOVE_Handler.hpp"
#include "WM_MOVE_Handler.hpp"
#include "WM_MOVING_Handler.hpp"
#include "WM_NCCALCSIZE_Handler.hpp"
#include "WM_NCMOUSELEAVE_Handler.hpp"
#include "WM_NCPAINT_Handler.hpp"
#include "WM_PAINT_Handler.hpp"
#include "WM_QUIT_Handler.hpp"
#include "WM_RBUTTONDOWN_Handler.hpp"
#include "WM_RBUTTONUP_Handler.hpp"
#include "WM_SETTEXT_Handler.hpp"
#include "WM_SIZE_Handler.hpp"
#include "WM_SIZING_Handler.hpp"
#include "WM_TIMER_Handler.hpp"
#include "WM_WINDOWPOSCHANGING_Handler.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

WindowsMessageHandlingService::WindowsMessageHandlingService()
{
	// Add all the message handlers to the message handler map
	_message_handler_map.insert(std::make_pair(WM_ACTIVATE, new WM_ACTIVATE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_CAPTURECHANGED, new WM_CAPTURECHANGED_Handler()));
	_message_handler_map.insert(std::make_pair(WM_CLOSE, new WM_CLOSE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_DESTROY, new WM_DESTROY_Handler()));
	_message_handler_map.insert(std::make_pair(WM_ENTERSIZEMOVE, new WM_ENTERSIZEMOVE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_ERASEBKGND, new WM_ERASEBKGND_Handler()));
	_message_handler_map.insert(std::make_pair(WM_EXITSIZEMOVE, new WM_EXITSIZEMOVE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_GETICON, new WM_GETICON_Handler()));
	_message_handler_map.insert(std::make_pair(WM_GETMINMAXINFO, new WM_GETMINMAXINFO_Handler()));
	_message_handler_map.insert(std::make_pair(WM_INPUT, new WM_INPUT_Handler()));
	_message_handler_map.insert(std::make_pair(WM_KEYUP, new WM_KEYUP_Handler()));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONDOWN, new WM_LBUTTONDOWN_Handler()));
	_message_handler_map.insert(std::make_pair(WM_LBUTTONUP, new WM_LBUTTONUP_Handler()));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONDOWN, new WM_MBUTTONDOWN_Handler()));
	_message_handler_map.insert(std::make_pair(WM_MBUTTONUP, new WM_MBUTTONUP_Handler()));
	_message_handler_map.insert(std::make_pair(WM_MENUCHAR, new WM_MENUCHAR_Handler()));
	_message_handler_map.insert(std::make_pair(WM_MOUSEMOVE, new WM_MOUSEMOVE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_MOVE, new WM_MOVE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_MOVING, new WM_MOVING_Handler()));
	_message_handler_map.insert(std::make_pair(WM_NCCALCSIZE, new WM_NCCALCSIZE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_NCMOUSELEAVE, new WM_NCMOUSELEAVE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_NCPAINT, new WM_NCPAINT_Handler()));
	_message_handler_map.insert(std::make_pair(WM_PAINT, new WM_PAINT_Handler()));
	_message_handler_map.insert(std::make_pair(WM_QUIT, new WM_QUIT_Handler()));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONDOWN, new WM_RBUTTONDOWN_Handler()));
	_message_handler_map.insert(std::make_pair(WM_RBUTTONUP, new WM_RBUTTONUP_Handler()));
	_message_handler_map.insert(std::make_pair(WM_SETTEXT, new WM_SETTEXT_Handler()));
	_message_handler_map.insert(std::make_pair(WM_SIZE, new WM_SIZE_Handler()));
	_message_handler_map.insert(std::make_pair(WM_SIZING, new WM_SIZING_Handler()));
	_message_handler_map.insert(std::make_pair(WM_TIMER, new WM_TIMER_Handler()));
	_message_handler_map.insert(std::make_pair(WM_WINDOWPOSCHANGING, new WM_WINDOWPOSCHANGING_Handler()));
}

WindowsMessageHandlingService::~WindowsMessageHandlingService()
{
	// clean up all those message handlers
	for (auto& pair : _message_handler_map)
	{
		delete pair.second;
	}
}

LRESULT WindowsMessageHandlingService::handle_message(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
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

