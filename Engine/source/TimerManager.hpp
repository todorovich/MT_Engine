//***************************************************************************************
// TimerManager.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#pragma once

#include "precompiled.hpp"

using steady_clock	= std::chrono::steady_clock;
using period		= std::chrono::steady_clock::period;
using duration		= std::chrono::steady_clock::duration;
using time_point	= std::chrono::steady_clock::time_point;

using namespace std::literals::chrono_literals;

namespace mt
{
	class TimerManager
	{
	
	public:
		TimerManager() = default;
	
		std::chrono::nanoseconds total_time() const { return _total_time; };
	
		std::chrono::nanoseconds run_time() const { return _unpaused_time; };
			
		std::chrono::nanoseconds paused_time() const { return _paused_time; };
		
		std::chrono::nanoseconds delta_time() const { return _delta_time_ns; };
		
		std::chrono::nanoseconds ns_until_next_update() const { return _time_until_next_update_ns; };
		
		std::chrono::nanoseconds ns_until_next_render() const { return _time_until_next_render_ns; };
		
		std::chrono::nanoseconds ns_until_next_idle() const { return _time_until_next_idle_ns; };
	
		std::chrono::duration<float, std::nano> avg_ns_between_update() const { return _avg_update_interval_ns; };
	
		std::chrono::duration<float, std::nano> avg_ns_between_render() const { return _avg_render_interval_ns; };
	
		std::chrono::duration<float, std::nano> avg_ns_per_update() const { return _avg_update_length_ns; };
	
		std::chrono::duration<float, std::nano> avg_ns_per_render() const { return _avg_render_length_ns; };
	
		std::chrono::duration<float, std::nano> command_list_interval() const { return _command_list_interval; };

		std::chrono::duration<float, std::nano> avg_ns_idle_per_interval() const { return _avg_idle_length_ns; }
	
		std::chrono::duration<float, std::nano> ns_idle_this_interval() const { return _idle_time_ns; }
	
		bool is_paused() const { return _is_paused;	}
	
		
		void Initialize(); // Call before message loop.
		void unpause_time();	 // Call when unpaused.
		void pause_time();		 // Call when paused.
		void tick();			 // Call every frame.
	
		// Mark the beginning of a update frame
		void start_update_timer();
		// Mark the end of an update Frame
		void end_update_timer();
	
		void start_render_timer();
		void end_render_timer();

		void start_command_list_timer();
		void end_command_list_timer();
	
		void start_new_idle_interval();
	
	private:
	
		static const float		_number_of_samples_to_use;
		
		void _calculate_idle_time();
		void _process_time();

		duration	_tgt_update_interval_ns =  8333333ns;
		duration	_tgt_render_interval_ns = 16666666ns;
		duration	_idle_time_interval_ns  = 16666666ns; // What is a frame? 16.66ms
		duration	_command_list_interval = 0ns;
	
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
		duration _unpaused_time = 0ns;
		duration _paused_time	= 0ns;
	
		duration _time_until_next_update_ns	  = 0ns;
		duration _time_until_next_render_ns	  = 0ns;
		duration _time_until_next_idle_ns	  = 0ns;
		
		duration _delta_time_ns	 = 0ns;
	
		duration _update_time_ns = 5ms;
		duration _render_time_ns = 5ms;
		duration _idle_time_ns	 = _idle_time_interval_ns - _update_time_ns - _render_time_ns;
	
		duration _update_this_idle = 0ns;
		duration _render_this_idle = 0ns;
	
		std::chrono::duration<float, std::nano> _avg_update_interval_ns = _tgt_update_interval_ns;
		std::chrono::duration<float, std::nano> _avg_render_interval_ns = _tgt_render_interval_ns;
	
		std::chrono::duration<float, std::nano> _avg_update_length_ns = _update_time_ns;
		std::chrono::duration<float, std::nano> _avg_render_length_ns = _render_time_ns;
		std::chrono::duration<float, std::nano> _avg_idle_length_ns	  = _idle_time_ns;
	
		bool _is_paused = true;
	
	};
}