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

struct Indenter
{
	int tab_count = 0;

	Indenter& operator++(int)
	{
		tab_count++;

		return *this;
	}

	Indenter& operator--(int)
	{
		tab_count--;

		return *this;
	}

	friend ostream& operator<<(ostream& out, const Indenter& tab);
};

ostream& operator<<(ostream& out, const Indenter& tab)
{
	for (auto i = 0; i < tab.tab_count; i++)
	{
		out << '\t';
	}

	return out;
}

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
	//_engine_tick_thread = std::thread(std::ref(Engine::Tick));
		
	// Message handler must be on same thread as the window (this thread)
	MSG msg = { 0 };

	auto& in_between_ticks_chrono = GetTimerManager().FindTimer(std::string("In Between Ticks Chonometer"));
	in_between_ticks_chrono.Start();
	{
		bool quit = false;
		while (!quit)
		{
			auto& windows_message_chrono = GetTimerManager().FindTimer(std::string("Windows Message Chronometer"));
			windows_message_chrono.Start();
			{
				// If there are Window messages then process them.
				while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					if (msg.message == WM_QUIT)
					{
						quit = true;
					}
				}
			}
			windows_message_chrono.Stop();

			in_between_ticks_chrono.Stop();

			GetEngine()._Tick();

			in_between_ticks_chrono.Start();
		};
	}
	in_between_ticks_chrono.Stop();
	// Join the Tick thread (ensuring it has actually shut down)
	//if (_engine_tick_thread.joinable()) _engine_tick_thread.join();
	
	//OutputDebugStringW(L"Engine Has Stopped.\n");

	return Status::success;
}

void Engine::_Tick()
{
	bool was_rendered = false;

	GetTimerManager().GetTickTimer().Start();
	{
		GetTimerManager().Tick();

		// Time for another Update
		//if (GetTimerManager().ns_until_next_update() <= 0ns)
		{

			auto& input_timer = GetTimerManager().FindTimer(std::string("Input Chronometer"));

			input_timer.Start();
			{
				_input_manager.ProcessInput();
			}
			input_timer.Stop();

			GetTimerManager().GetUpdateTimer().Start();
			{
				Update();
				_direct_x_renderer.Update();
			}
			GetTimerManager().GetUpdateTimer().Stop();
		}

		// End of Frame
		// New Frame
		// Time to Render
		//if (GetTimerManager().ns_until_next_frame() <= 0ns)
		{
			// Render whenever you can, but don't wait.
			if (!GetTimerManager().IsRenderPaused() && GetRenderer().IsCurrentFenceComplete())
			{
				GetTimerManager().GetRenderTimer().Start();
				{
					Draw();
					_direct_x_renderer.Render();
					_direct_x_renderer.IncrementFence();
					was_rendered = true;
				}
				GetTimerManager().GetRenderTimer().Stop();
			}
		}

	}
	GetTimerManager().GetTickTimer().Stop();

	_UpdateFrameStatisticsNoTimeCheck(was_rendered);
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

	if(!_direct_x_renderer.InitializeDirect3d(_main_window_handle))
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

//void Engine::_UpdateFrameStatistics()
//{
//	// Code computes the average frames per second, and also the 
//	// average _time_manager it takes to render one frame.  These stats 
//	// are appended to the window caption bar.
//	
//	_time_since_stat_update += GetTimerManager().DeltaTime()
//
//	if (_time_since_stat_update >= 250ms)
//	{
//		_UpdateFrameStatisticsNoTimeCheck();
//	}		
//}

void Engine::_UpdateFrameStatisticsNoTimeCheck(bool was_rendered)
{
	GetTimerManager().GetStatisticsTimer().Start();

	static const float ns_to_ms = 1e-6f;

	auto& game_chrono		= Engine::GetTimerManager().GetGameTimer();
	auto& update_chrono		= Engine::GetTimerManager().GetUpdateTimer();
	auto& render_chrono		= Engine::GetTimerManager().GetRenderTimer();
	auto& wm_chrono			= Engine::GetTimerManager().FindTimer(std::string("Windows Message Chronometer"));
	auto& in_between_chrono = Engine::GetTimerManager().FindTimer(std::string("In Between Ticks Chonometer"));
	auto& stats_chrono		= Engine::GetTimerManager().GetStatisticsTimer();
	auto& tick_chrono		= Engine::GetTimerManager().GetTickTimer();
	//auto& frame_timer = Engine::GetTimerManager().GetFrameTimer();

	float ns_to_s = 1e-9f;

	const auto emsp = "&emsp;";
	const auto red_span = "<span style=\"color:Red\">";
	const auto span_end = "</span>";

	auto& log_ostream = GetLogManager().GetMainLog().GetOStream();
	Indenter indenter;

	log_ostream	<< '\n' << "<table>" << '\n';

	indenter++;
	log_ostream	<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream << indenter << "<td style=\"vertical-align:top\">" << '\n';

	indenter++;
	log_ostream	<< indenter << "<table>" << '\n';
	
	indenter++;
	log_ostream << indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<th></th>" << '\n'
		<< indenter << "<th>Duration Active</th>" << '\n'
		<< indenter << "<th>Duration Paused</th>" << '\n'
		<< indenter << "<th>Total Duration</th>" << '\n';

	indenter--;
	log_ostream
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<td>" << game_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetDurationActive().count()		* ns_to_s << "s" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetDurationPaused().count()		* ns_to_s << "s" << "</td>" << '\n'
		<< indenter << "<td>" << game_chrono.GetTotalDurationRunning().count()	* ns_to_s << "s" << "</td>" << '\n';
	
	indenter--;
	log_ostream
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<td>" << update_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetDurationActive().count()			* ns_to_ms << "ns" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetDurationPaused().count()			* ns_to_ms << "ns" << "</td>" << '\n'
		<< indenter << "<td>" << update_chrono.GetTotalDurationRunning().count()	* ns_to_ms << "ns" << "</td>" << '\n';
		
	indenter--;
	log_ostream
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<td>" << render_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetDurationActive().count()			* ns_to_ms << "ns" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetDurationPaused().count()			* ns_to_ms << "ns" << "</td>" << '\n'
		<< indenter << "<td>" << render_chrono.GetTotalDurationRunning().count()	* ns_to_ms << "ns" << "</td>" << '\n';


	indenter--;
	log_ostream
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<td>" << tick_chrono.GetName() << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetDurationActive().count()	 * ns_to_ms << "ns" << "</td>" << '\n'
		<< indenter << "<td>" << tick_chrono.GetDurationPaused().count()	 * ns_to_ms << "ns" << "</td>" << '\n';

	auto temp = tick_chrono.GetTotalDurationRunning().count() * ns_to_ms;

	if (temp >= 16)
	{
		log_ostream	<< indenter << "<td>" << red_span << temp << span_end << "ns" << "</td>" << '\n';
	}
	else
	{
		log_ostream	<< indenter << "<td>" << temp << "ns" << "</td>" << '\n';
	}

	indenter--;
	log_ostream << indenter << "</tr>" << '\n';
	
	indenter--;
	log_ostream << indenter << "</table>" << '\n';

	indenter--;
	log_ostream
		<< indenter << "</td>" << '\n'
		<< indenter << "<td style=\"vertical-align:top\">" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<table>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<th></th>" << '\n'
		<< indenter << "<th>Update</th>" << '\n'
		<< indenter << "<th>Render</th>" << '\n'
		<< indenter << "<th>Windows Messages</th>" << '\n'
		<< indenter << "<th>Statistics</th>" << '\n'
		<< indenter << "<th>Tick</th>" << '\n'
		<< indenter << "<th>In-Between Ticks</th>" << '\n';

	indenter--;
	log_ostream
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<td>Average</td>" << '\n'
		<< indenter << "<td>" << (update_chrono.GetAverageSampleDuration().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (was_rendered ? (render_chrono.GetAverageSampleDuration().count() * ns_to_ms) : 0) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (wm_chrono.GetAverageSampleDuration().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (stats_chrono.GetAverageSampleDuration().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (tick_chrono.GetAverageSampleDuration().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (in_between_chrono.GetAverageSampleDuration().count() * ns_to_ms) << "ms" << "</td>" << '\n';

	indenter--;
	log_ostream
		<< indenter << "</tr>" << '\n'
		<< indenter << "<tr>" << '\n';

	indenter++;
	log_ostream
		<< indenter << "<td>Last Sample</td>" << '\n'
		<< indenter << "<td>" << (update_chrono.GetLastSample().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (render_chrono.GetLastSample().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (wm_chrono.GetLastSample().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (stats_chrono.GetLastSample().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (tick_chrono.GetLastSample().count() * ns_to_ms) << "ms" << "</td>" << '\n'
		<< indenter << "<td>" << (in_between_chrono.GetLastSample().count() * ns_to_ms) << "ms" << "</td>" << '\n';

	indenter--;
	log_ostream	<< indenter << "</tr>" << '\n';

	indenter--;
	log_ostream	<< indenter << "</table>" << '\n';

	indenter--;
	log_ostream	<< indenter << "</td>" << '\n';

	indenter--;
	log_ostream	<< indenter << "</tr>" << '\n';

	indenter--;
	log_ostream	<< indenter << "</table>" << '\n';

	_time_since_stat_update = 0ns;

	GetTimerManager().GetStatisticsTimer().Stop();
}