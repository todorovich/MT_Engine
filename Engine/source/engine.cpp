 
#include "engine.hpp"
#include <WindowsX.h>
#include <thread>

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


#include <windows.h>  
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)  
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.  
	LPCSTR szName; // Pointer to name (in user addr space).  
	DWORD dwThreadID; // Thread ID (-1=caller thread).  
	DWORD dwFlags; // Reserved for future use, must be zero.  
} THREADNAME_INFO;
#pragma pack(pop)  
void SetThreadName(DWORD dwThreadID, const char* threadName) {
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
#pragma warning(push)  
#pragma warning(disable: 6320 6322)  
	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
#pragma warning(pop)  
}


using namespace mt;

std::unique_ptr<engine> mt::engine::_instance = std::make_unique<engine>();

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return engine::GetWindowsManager().handle_message(hwnd, msg, wParam, lParam);
	
}

Status engine::_run()
{ 
	_time_manager.Initialize();

	//_engine_tick_thread = std::thread(std::ref(engine::GetEngine().tick));
		
	// Message handler must be on same thread as the window (this thread)
	MSG msg = { 0 };
	
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GetEngine()._tick();

	}

	// Join the tick thread (ensuring it has actually shut down)
	//if (_engine_tick_thread.joinable()) _engine_tick_thread.join();

	return Status::success;
}

void engine::_tick()
{
	_time_manager.tick();

	// Check to see if we are running, and its _time_manager to Update, if so then Update.
	if (_time_manager.is_paused() != true && _time_manager.ns_until_next_update() <= 0ns)
	{
		this->GetInputHandler().ProcessInput();

		_time_manager.start_update_timer();
		Update();
		_direct_x_renderer.Update();
		_time_manager.end_update_timer();
	}

	// Check to see if we are not resizing, and that it is _time_manager to Render, if so then Render
	if (!_is_window_resizing && _time_manager.ns_until_next_render() <= 0ns)
	{
		_time_manager.start_render_timer();
		Draw();
		_direct_x_renderer.render();
		_time_manager.end_render_timer();
	}

	if (_time_manager.ns_until_next_idle() <= 0ns)
	{
		_direct_x_renderer.flush_command_queue();
		_time_manager.start_new_idle_interval();
		_update_frame_stats();
	}

}

void engine::tick()
{
	SetThreadName(GetCurrentThreadId(), "Tick Thread");

	while (!GetEngine()._should_shutdown)
	{
		GetEngine()._tick();
	}

	OutputDebugStringW(L"Engine Shutdown\n");
}

bool engine::_initialize(HINSTANCE hInstance)
{
	_instance_handle = hInstance;
		
	if(!_init_main_window())
		return false;

	if(!_direct_x_renderer.initialize_direct3d(_main_window_handle))
		return false;

	// Do the initial Resize code.
	Resize(_window_width, _window_height);

	return true;
}

bool engine::_init_main_window()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = _instance_handle;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"MainWnd";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, _window_width, _window_height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	_main_window_handle = CreateWindow(L"MainWnd", _main_window_caption.c_str(), 
		WS_MAXIMIZE, 0, 0, width, height, nullptr, nullptr, _instance_handle, 0);

	if( !_main_window_handle )
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}
	

	SetWindowLong(_main_window_handle, GWL_STYLE, 0); //remove all window styles, check MSDN for details

	ShowWindow(_main_window_handle, SW_SHOWMAXIMIZED); //display window
	
	UpdateWindow(_main_window_handle);

	return true;
}

void engine::_update_frame_stats()
{
	// Code computes the average frames per second, and also the 
	// average _time_manager it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	
	_time_since_stat_update += _time_manager.delta_time();

	if (_time_since_stat_update >= 250ms)
	{
		static const float ten_e_neg_six = 1.0f / 1000000.0f;

		wstring mspr = to_wstring(_time_manager.avg_ns_per_render().count() * ten_e_neg_six);
		wstring mscl = to_wstring(_time_manager.command_list_interval().count() * ten_e_neg_six);
		wstring mspf = to_wstring(_time_manager.avg_ns_per_update().count() * ten_e_neg_six);

		wstring  avg_ms_idle_per_interval = to_wstring(_time_manager.avg_ns_idle_per_interval().count() * ten_e_neg_six);
		wstring  ms_idle_per_interval	  = to_wstring(_time_manager.ns_idle_this_interval().count() * ten_e_neg_six);

		wstring rfps = to_wstring(1.0s / _time_manager.avg_ns_between_render());
		wstring ufps = to_wstring(1.0s / _time_manager.avg_ns_between_update());

		wstring windowText = _main_window_caption +
			L"   ms/command: " + mscl +
			L"   ms/render: " + mspr + L" Render FPS: " + rfps +
			L"   ms/update: " + mspf + L" Update FPS: " + ufps + 
			L"   ms idle: "   + ms_idle_per_interval + L" avg ms idle: " + avg_ms_idle_per_interval + L'\n';

		//SetWindowText(_main_window_handle, windowText.c_str());
		
		OutputDebugStringW((LPWSTR)windowText.c_str());

		_time_since_stat_update = 0ns;
	}		
}