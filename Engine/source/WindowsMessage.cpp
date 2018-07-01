#include "WindowsMessage.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

LRESULT WindowsMessage::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam) { return 0; };