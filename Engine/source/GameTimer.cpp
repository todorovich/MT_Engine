//***************************************************************************************
// GameTimer.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

//#include <windows.h>
#include <chrono>
#include "GameTimer.hpp"
#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

const float GameTimer::_number_of_samples_to_use = 100.0f;

void GameTimer::initialize()
{
	_curr_tick_time	 = steady_clock::now();
	_prev_tick_time	 = _curr_tick_time;

	_curr_render_start_time = steady_clock::now();
	_curr_update_start_time = steady_clock::now();
	_curr_idle_start_time   = steady_clock::now();

	_prev_render_start_time = steady_clock::now();
	_prev_update_start_time = steady_clock::now();
	_prev_idle_start_time	= steady_clock::now();

	_total_time	   = 0ns;
	_paused_time   = 0ns;
	_unpaused_time = 0ns;

	_time_until_next_update_ns	= 0ns;
	_time_until_next_render_ns	= 0ns;
	_time_until_next_idle_ns	= 0ns;

	_delta_time_ns	= 0ns;

	_update_time_ns = 5ms;
	_render_time_ns = 5ms;
	_idle_time_ns	= _idle_time_interval_ns - _update_time_ns - _render_time_ns;

	_update_this_idle = 0ns;
	_render_this_idle = 0ns;

	_avg_update_interval_ns = _tgt_update_interval_ns;
	_avg_render_interval_ns = _tgt_render_interval_ns;

	_avg_update_length_ns	= _update_time_ns;
	_avg_render_length_ns	= _render_time_ns;
	_avg_idle_length_ns		= _idle_time_ns;

	_is_paused	 = false;
}

void GameTimer::start_update_timer()
{
	_prev_update_start_time = _curr_update_start_time;
	_curr_update_start_time = steady_clock::now();

	_time_until_next_update_ns += _tgt_update_interval_ns;
}

void GameTimer::end_update_timer()
{
	_update_time_ns = steady_clock::now() - _curr_update_start_time;

	auto update_interval = _curr_update_start_time - _prev_update_start_time;

	_avg_update_interval_ns -= _avg_update_interval_ns / _number_of_samples_to_use;
	_avg_update_interval_ns += update_interval / _number_of_samples_to_use;

	_avg_update_length_ns -= _avg_update_length_ns / _number_of_samples_to_use;
	_avg_update_length_ns += _update_time_ns / _number_of_samples_to_use;

	_update_this_idle += _update_time_ns;
}

void GameTimer::start_render_timer()
{
	_prev_render_start_time = _curr_render_start_time;
	_curr_render_start_time = steady_clock::now();

	_time_until_next_render_ns += _tgt_render_interval_ns;
}

void GameTimer::end_render_timer()
{
	_render_time_ns = steady_clock::now() - _curr_render_start_time;

	auto render_interval = _curr_render_start_time - _prev_render_start_time;

	_avg_render_interval_ns -= _avg_render_interval_ns / _number_of_samples_to_use;
	_avg_render_interval_ns += render_interval / _number_of_samples_to_use;

	//_avg_render_length_ns -= _avg_render_length_ns / _number_of_samples_to_use;
	//_avg_render_length_ns += _render_time_ns / _number_of_samples_to_use;

	_avg_render_length_ns = _render_time_ns;

	_render_this_idle += _render_time_ns;

}

void mt::GameTimer::start_command_list_timer()
{
	_command_list_start_time = steady_clock::now();
}

void mt::GameTimer::end_command_list_timer()
{
	_command_list_interval = steady_clock::now() - _curr_render_start_time;
}

void GameTimer::start_new_idle_interval()
{
	// Push the current idle start time to previous
	// and Record the start of this idle frame
	_prev_idle_start_time = _curr_idle_start_time;
	_curr_idle_start_time = steady_clock::now();

	// Calculate the actual amount of time spent idle this frame.
	_idle_time_ns = (_curr_idle_start_time - _prev_idle_start_time) - _update_time_ns - _render_time_ns;

	// Update the rolling sample by removing an average sample 
	// and adding a sample for this frame
	_avg_idle_length_ns -= _avg_idle_length_ns / _number_of_samples_to_use;
	_avg_idle_length_ns += _idle_time_ns / _number_of_samples_to_use;

	// Add the idle time interval to time until next, effectively reseting idle time this frame
	// while still accounting for any time we went over frame
	// This is used to keep timing inaccuracies from stacking into the math
	_time_until_next_idle_ns += _idle_time_interval_ns;

	// reset update and render time
	_update_this_idle = 0ns;
	_render_this_idle = 0ns;

}

void GameTimer::_process_time()
{
	_prev_tick_time = _curr_tick_time;
	_curr_tick_time = steady_clock::now();

	_delta_time_ns  = _curr_tick_time - _prev_tick_time;

	_total_time += _delta_time_ns;

	_time_until_next_idle_ns   -= _delta_time_ns;

}

void GameTimer::unpause_time()
{
	// If the game is paused, unpause it.
	if (_is_paused == true)
	{
		_is_paused = false;
	}

	// else do nothing
}

void GameTimer::pause_time()
{
	// If the game is not paused, pause it.
	if(_is_paused == false)
	{
		_is_paused = true;
	}

	// else do nothing
}

void GameTimer::tick()
{
	_process_time();

	// IS PAUSED
	if (engine::get_engine().get_is_resizing())
	{
		_paused_time += _delta_time_ns;
	}
	else
	{
		if (_is_paused)
		{
			_paused_time += _delta_time_ns;
		}
		// NOT paused
		else
		{
			_unpaused_time += _delta_time_ns;

			_time_until_next_update_ns -= _delta_time_ns;
		}

		_time_until_next_render_ns -= _delta_time_ns;
	}
}

