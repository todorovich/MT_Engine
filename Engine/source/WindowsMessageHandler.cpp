#include "WindowsMessageHandler.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

LRESULT WindowsMessageHandler::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam) { return 0; };

//void WindowsMessageHandler::tick_engine()
//{
//	mt_engine::tick();
//}
//
//void WindowsMessageHandler::resize_engine(int width, int height)
//{
//	mt_engine::resize(width, height);
//}
//
//void WindowsMessageHandler::set_is_resizing_engine(bool is_resizing)
//{
//	mt_engine::set_is_resizing(is_resizing);
//}
//
//GameTimer& WindowsMessageHandler::get_game_timer()
//{
//	return mt_engine::get_game_timer();
//}
//
//void WindowsMessageHandler::OnMouseDown(WPARAM btnState, int x, int y)
//{
//	mt_engine::on_mouse_down(btnState, x, y);
//}
//
//void WindowsMessageHandler::OnMouseUp(WPARAM btnState, int x, int y)
//{
//	mt_engine::on_mouse_up(btnState, x, y);
//}
//
//void WindowsMessageHandler::OnMouseMove(WPARAM btnState, int x, int y)
//{
//	mt_engine::on_mouse_move(btnState, x, y);
//}
//
//void WindowsMessageHandler::set_is_minimized_engine(bool is_minimized)
//{
//	mt_engine::set_is_minimized(is_minimized);
//}
//
//void WindowsMessageHandler::set_is_maximized_engine(bool is_maximized)
//{
//	mt_engine::set_is_maximized(is_maximized);
//}
//
//DirectXRenderer& WindowsMessageHandler::get_renderer()
//{
//	return mt_engine::get_renderer();
//}
//
//bool WindowsMessageHandler::get_engine_is_resizing()
//{
//	return mt_engine::get_engine()->get_is_resizing();
//}

