// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"
#include "Timer.hpp"

using steady_clock	= std::chrono::steady_clock;
using period		= std::chrono::steady_clock::period;
using duration		= std::chrono::steady_clock::duration;
using time_point	= std::chrono::steady_clock::time_point;

using namespace std::literals::chrono_literals;

namespace mt
{
	class TimerManager
	{
		static const int		_number_of_samples_to_use;
	
	public:
		TimerManager()
			: _tgt_update_interval_ns(8333333ns)
			, _tgt_render_interval_ns(16666666ns)
			, _idle_time_interval_ns (16666666ns)
			, _command_list_interval (0ns)
			, _time_until_next_update_ns(8333333ns)
			, _time_until_next_render_ns(16666666ns)
			, _is_paused(true)
		{}

		~TimerManager()
		{
			_DeleteAllTimers();
		}
	
		time_point GetCurrentTickTime() const { return _curr_tick_time; };

		time_point GetPreviousTickTime() const { return _prev_tick_time; };

		duration DurationSinceLaunch() const { return _timers.find(std::wstring(L"Total Game Time"))->second->GetTotalDurationRunning(); };
	
		duration DurationActive() const { return _duration_active; };
			
		duration DurationUpdatePaused() const { return _duration_active; };
		
		duration DeltaTime() const { return _tick_delta_time_ns; };

		duration ns_until_next_update() const { return _time_until_next_update_ns; };
		
		duration ns_until_next_render() const { return _time_until_next_render_ns; };
		
		duration ns_until_next_frame() const { return _time_until_next_frame_ns; };

		duration avg_ns_between_update() const { return _avg_update_interval_ns; };

		duration avg_ns_between_render() const { return _avg_render_interval_ns; };

		duration avg_ns_per_update() const { return _avg_update_length_ns; };

		duration avg_ns_per_render() const { return _avg_render_length_ns; };

		duration command_list_interval() const { return _command_list_interval; };

		duration avg_ns_idle_per_interval() const { return _avg_idle_length_ns; }

		duration ns_idle_this_interval() const { return _idle_time_ns; }
			
		void Initialize();		// Call before message loop.
		void Continue();		// Call to unpaused.
		void Pause();			// Call to paused.
		void Tick();			// Call every frame.
	
		Timer&  GetGameTimer() { return *_timers.find(std::wstring(L"Total Game Time"))->second; }
		Timer&  GetUpdateTimer() { return *_timers.find(std::wstring(L"Update Timer"))->second; }
		Timer&  GetRenderTimer() { return *_timers.find(std::wstring(L"Render Timer"))->second; }
		Timer&  GetStatisticsTimer() { return *_timers.find(std::wstring(L"Statistics Timer"))->second; }
		Timer&  GetTickTimer() { return *_timers.find(std::wstring(L"Tick Timer"))->second; }
		
		void start_update_timer();
		void end_update_timer();
	
		void start_render_timer();
		void end_render_timer();

		void start_command_list_timer();
		void end_command_list_timer();
	
		void start_new_idle_interval();

		bool IsUpdatePaused() const { return _is_paused; }
		bool IsRenderPaused() const { return _is_render_paused; }
	
	private:

		void _DeleteAllTimers();
		void _AddEngineTimers();
		void _StartAllTimers();

		std::map<std::wstring, Timer*>		_timers;

		duration	_tgt_update_interval_ns;
		duration	_tgt_render_interval_ns;
		duration	_idle_time_interval_ns;
		duration	_command_list_interval;
	
		time_point _curr_tick_time = time_point(0ns);
		time_point _prev_tick_time = time_point(0ns);
	
		time_point _curr_render_start_time	= time_point(0ns);
		time_point _prev_render_start_time	= time_point(0ns);
	
		time_point _command_list_start_time = time_point(0ns);

		time_point _curr_update_start_time	= time_point(0ns);
		time_point _prev_update_start_time	= time_point(0ns);
	
		time_point _curr_idle_start_time = time_point(0ns);
		time_point _prev_idle_start_time = time_point(0ns);
	
		duration _total_time	= 0ns;
		duration _duration_active = 0ns;
		duration _duration_paused	= 0ns;
	
		duration _time_until_next_update_ns	  = 0ns;
		duration _time_until_next_render_ns	  = 0ns;
		duration _time_until_next_frame_ns	  = 0ns;
		
		duration _tick_delta_time_ns	 = 0ns;
	
		duration _update_time_ns = 5ms;
		duration _render_time_ns = 5ms;
		duration _idle_time_ns	 = _idle_time_interval_ns - _update_time_ns - _render_time_ns;
	
		duration _update_this_idle = 0ns;
		duration _render_this_idle = 0ns;
	
		duration _avg_update_interval_ns = _tgt_update_interval_ns;
		duration _avg_render_interval_ns = _tgt_render_interval_ns;
	
		duration _avg_update_length_ns = _update_time_ns;
		duration _avg_render_length_ns = _render_time_ns;
		duration _avg_idle_length_ns	  = _idle_time_ns;
	
		bool _is_paused;
		bool _is_render_paused;
	
	};
}