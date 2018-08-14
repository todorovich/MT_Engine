// Copyright 2018 Micho Todorovich, all rights reserved.

#include "engine.hpp"

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)  
struct THREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.  
	LPCSTR szName; // Pointer to _name (in user addr space).  
	DWORD dwThreadID; // Thread ID (-1=caller thread).  
	DWORD dwFlags; // Reserved for future use, must be zero.  
};
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

std::unique_ptr<Engine> mt::Engine::_instance = std::make_unique<Engine>();

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return Engine::GetWindowsMessageManager().handle_message(hwnd, msg, wParam, lParam);
	
}

Status Engine::_Run()
{ 
	namespace fs = std::filesystem;
	fs::path p = fs::current_path();

	OutputDebugStringW(wstring(
		L"The current path " + p.wstring() + L" decomposes into:\n"
		+ L"root name " + p.root_name().wstring() + L'\n'
		+ L"root directory " + p.root_directory().wstring() + L'\n'
		+ L"relative path " + p.relative_path().wstring() + L'\n'
		).c_str());

	//_engine_tick_thread = std::thread(std::ref(Engine::Tick));
		
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

		GetEngine()._Tick();
	};

	// Join the Tick thread (ensuring it has actually shut down)
	//if (_engine_tick_thread.joinable()) _engine_tick_thread.join();
	
	OutputDebugStringW(L"Engine Has Stopped.\n");

	return Status::success;
}

void Engine::_Tick()
{
	GetTimerManager().GetTickTimer().Start();

	GetTimerManager().Tick();

	// Time for another Update
	if (GetTimerManager().ns_until_next_update() <= 0ns)
	{
		_input_manager.ProcessInput();
		_time_manager.start_update_timer();
		Update();
		_direct_x_renderer.Update();
		_time_manager.end_update_timer();
	}

	// End of Frame
	if (_time_manager.ns_until_next_frame() <= 0ns)
	{
		// Render at the end of the frame
		if (!GetTimerManager().IsRenderPaused())
		{
			GetTimerManager().start_render_timer();
			Draw();
			_direct_x_renderer.render();
			GetTimerManager().end_render_timer();
		}

		_direct_x_renderer.flush_command_queue();
		GetTimerManager().start_new_idle_interval();
	}

	//_UpdateFrameStatistics();

	auto time = GetTimerManager().GetTickTimer().Stop();
	if (time  > 16ms)
	{
		_UpdateFrameStatisticsNoTimeCheck();
		//OutputDebugStringW(L"\nBAM!"); 
		//OutputDebugStringW(to_wstring(time.count()).c_str());
	}
}

void Engine::Tick()
{
	SetThreadName(GetCurrentThreadId(), "Tick Thread");

	while (!GetEngine()._is_shutting_down)
	{
		GetEngine()._Tick();
	}

	OutputDebugStringW(L"Tick Thread Shutdown\n");
}

bool Engine::_Initialize(HINSTANCE hInstance)
{
	_instance_handle = hInstance;
		
	if(!_InitializeMainWindow())
		return false;

	if(!_direct_x_renderer.initialize_direct3d(_main_window_handle))
		return false;

	// Do the initial Resize code.
	Resize(_window_width, _window_height);

	_time_manager.Initialize();

	return true;
}

bool Engine::_InitializeMainWindow()
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

void Engine::_UpdateFrameStatistics()
{
	// Code computes the average frames per second, and also the 
	// average _time_manager it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	
	_time_since_stat_update += _time_manager.DeltaTime();

	if (_time_since_stat_update >= 250ms)
	{
		_UpdateFrameStatisticsNoTimeCheck();
	}		
}

void Engine::_UpdateFrameStatisticsNoTimeCheck()
{
	GetTimerManager().GetStatisticsTimer().Start();

	static const float ten_e_neg_six = 1.0f / 1000000.0f;

	wstring mspr = to_wstring(_time_manager.avg_ns_per_render().count() * ten_e_neg_six);
	wstring mscl = to_wstring(_time_manager.command_list_interval().count() * ten_e_neg_six);
	wstring mspf = to_wstring(_time_manager.avg_ns_per_update().count() * ten_e_neg_six);

	wstring  avg_ms_idle_per_interval = to_wstring(_time_manager.avg_ns_idle_per_interval().count() * ten_e_neg_six);
	wstring  ms_idle_per_interval = to_wstring(_time_manager.ns_idle_this_interval().count() * ten_e_neg_six);

	wstring rfps = to_wstring(1.0s / _time_manager.avg_ns_between_render());
	wstring ufps = to_wstring(1.0s / _time_manager.avg_ns_between_update());

	auto& game_timer = Engine::GetTimerManager().GetGameTimer();
	auto& update_timer = Engine::GetTimerManager().GetUpdateTimer();
	auto& render_timer = Engine::GetTimerManager().GetRenderTimer();
	auto& stats_timer = Engine::GetTimerManager().GetStatisticsTimer();
	auto& tick_Timer = Engine::GetTimerManager().GetTickTimer();

	wstring windowText = _main_window_caption + L'\n' +
		L"   " + game_timer.GetName() +
		L"   Average Sample Duration: " + to_wstring(std::chrono::duration_cast<std::chrono::seconds>(game_timer.GetAverageSampleDuration()).count()) +
		L"   Duration Active: " + to_wstring(std::chrono::duration_cast<std::chrono::seconds>((game_timer.GetDurationActive())).count()) +
		L"   Duration Paused: " + to_wstring(std::chrono::duration_cast<std::chrono::seconds>(game_timer.GetDurationPaused()).count()) +
		L"   Total Duration: " + to_wstring(std::chrono::duration_cast<std::chrono::seconds>(game_timer.GetTotalDurationRunning()).count()) + L'\n' +

		L"   " + update_timer.GetName() +
		L"   Average Sample Duration: " + to_wstring(static_cast<double>(update_timer.GetAverageSampleDuration().count()) / 1'000'000.0) +
		L"   Duration Active: " + to_wstring(static_cast<double>(update_timer.GetDurationActive().count()) / 1'000'000.0) +
		L"   Duration Paused: " + to_wstring(static_cast<double>(update_timer.GetDurationPaused().count()) / 1'000'000.0) +
		L"   Total Duration: " + to_wstring(static_cast<double>(update_timer.GetTotalDurationRunning().count()) / 1'000'000.0) + L'\n' +

		L"   " + render_timer.GetName() +
		L"   Average Sample Duration: " + to_wstring(static_cast<double>(render_timer.GetAverageSampleDuration().count()) / 1'000'000.0) +
		L"   Duration Active: " + to_wstring(static_cast<double>(render_timer.GetDurationActive().count()) / 1'000'000.0) +
		L"   Duration Paused: " + to_wstring(static_cast<double>(render_timer.GetDurationPaused().count()) / 1'000'000.0) +
		L"   Total Duration: " + to_wstring(static_cast<double>(render_timer.GetTotalDurationRunning().count()) / 1'000'000.0) + L'\n' +

		L"   " + stats_timer.GetName() +
		L"   Average Sample Duration: " + to_wstring(static_cast<double>(stats_timer.GetAverageSampleDuration().count()) / 1'000'000.0) + L'\n' +

		L"   " + tick_Timer.GetName() +
		L"   Average Sample Duration: " + to_wstring(static_cast<double>(tick_Timer.GetAverageSampleDuration().count()) / 1'000'000.0) + L'\n' +
		L"   Average Sample Duration: " + to_wstring(static_cast<double>(tick_Timer.GetDurationActive().count()) / 1'000'000.0) + L'\n' +
		L"   Last Tick Duration: " + to_wstring(static_cast<double>(tick_Timer.GetLastSample().count()) / 1'000'000.0) + L'\n' +

		L"   ms/command: " + mscl +
		L"   ms/render: " + mspr + L" Render FPS: " + rfps +
		L"   ms/update: " + mspf + L" Update FPS: " + ufps +
		L"   ms idle: " + ms_idle_per_interval + L" avg ms idle: " + avg_ms_idle_per_interval + L'\n';

	//SetWindowText(_main_window_handle, windowText.c_str());

	OutputDebugStringW((LPWSTR)windowText.c_str());

	_time_since_stat_update = 0ns;

	GetTimerManager().GetStatisticsTimer().Stop();
}