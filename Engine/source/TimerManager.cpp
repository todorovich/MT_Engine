// Copyright 2018 Micho Todorovich, all rights reserved.

#include "TimerManager.hpp"
#include "engine.hpp"

using namespace mt;

const int TimerManager::_number_of_samples_to_use = 100;

void TimerManager::Initialize()
{
	_DeleteAllTimers();

	_AddEngineTimers();

	_StartAllTimers();


	_curr_tick_time	 = steady_clock::now();
	_prev_tick_time	 = time_point(0ns);

	_curr_render_start_time = _curr_tick_time;
	_curr_update_start_time = _curr_tick_time;
	_curr_idle_start_time   = _curr_tick_time;

	_prev_render_start_time = _prev_tick_time;
	_prev_update_start_time = _prev_tick_time;
	_prev_idle_start_time	= _prev_tick_time;

	_total_time			= 0ns;
	_duration_active	= 0ns;
	_duration_active	= 0ns;

	_time_until_next_update_ns	= 0ns;
	_time_until_next_render_ns	= 0ns;
	_time_until_next_frame_ns	= 0ns;

	_tick_delta_time_ns	= 0ns;

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

void TimerManager::Tick()
{
	_prev_tick_time = _curr_tick_time;

	_curr_tick_time = steady_clock::now();

	_tick_delta_time_ns = _curr_tick_time - _prev_tick_time;

	// Tick all the timers
	for (auto& timer : _timers)
	{
		timer.second->_Tick(_curr_tick_time, _prev_tick_time, _tick_delta_time_ns);
	}

	_total_time += _tick_delta_time_ns;

	_time_until_next_frame_ns -= _tick_delta_time_ns;

	if (_is_paused)
	{
		_duration_active += _tick_delta_time_ns;
	}
	else
	{
		_duration_active += _tick_delta_time_ns;
		_time_until_next_update_ns -= _tick_delta_time_ns;
	}
	
	if (_is_render_paused)
	{
		_time_until_next_render_ns = _tgt_render_interval_ns;
	}
	else
	{
		_time_until_next_render_ns -= _tick_delta_time_ns;		
	}
}

void TimerManager::Continue()
{
	// If the game is paused, unpause it.
	if (_is_paused == true)
	{
		_is_paused = false;

		auto continue_time = steady_clock::now();

		for (auto& pair : _timers)
		{
			Timer*& timer = pair.second;
			timer->_Continue(continue_time);
		}
	}

	// else do nothing
}

void TimerManager::Pause()
{
	// If the game is not paused, pause it.
	if(_is_paused == false)
	{
		_is_paused = true;

		auto time_paused = steady_clock::now();

		for (auto& pair : _timers)
		{
			Timer*& timer = pair.second;
			timer->_Pause(time_paused);
		}
	}

	// else do nothing
}

void TimerManager::_DeleteAllTimers()
{
	for (auto& pair : _timers)
	{
		Timer*& timer = pair.second;
		delete timer;
	}

	_timers.clear();
}

void TimerManager::_AddEngineTimers()
{
	
	auto timer = new Timer(L"Total Game Time");

	_timers.insert(std::make_pair(timer->GetName(), timer));

	timer = new Timer(L"Update Timer");

	_timers.insert(std::make_pair(timer->GetName(), timer));

	timer = new Timer(L"Render Timer");

	_timers.insert(std::make_pair(timer->GetName(), timer));

	timer = new Timer(L"Statistics Timer");

	_timers.insert(std::make_pair(timer->GetName(), timer));

	timer = new Timer(L"Frame Timer");

	_timers.insert(std::make_pair(timer->GetName(), timer));
	
	timer = new Timer(L"Tick Timer");

	_timers.insert(std::make_pair(timer->GetName(), timer));
}

void TimerManager::_StartAllTimers()
{
	for (auto& pair : _timers)
	{
		Timer*& timer = pair.second;
		timer->Start();
	}
}

void TimerManager::start_update_timer()
{
	GetUpdateTimer().Start();
	_prev_update_start_time = _curr_update_start_time;
	_curr_update_start_time = steady_clock::now();

	_time_until_next_update_ns += _tgt_update_interval_ns;
}

void TimerManager::end_update_timer()
{
	GetUpdateTimer().Stop();
	_update_time_ns = steady_clock::now() - _curr_update_start_time;

	auto update_interval = _curr_update_start_time - _prev_update_start_time;

	_avg_update_interval_ns -= static_cast<std::chrono::nanoseconds>(_avg_update_interval_ns /_number_of_samples_to_use);
	_avg_update_interval_ns += static_cast<std::chrono::nanoseconds>(update_interval / _number_of_samples_to_use);

	_avg_update_length_ns -= _avg_update_length_ns / _number_of_samples_to_use;
	_avg_update_length_ns += _update_time_ns / _number_of_samples_to_use;

	_update_this_idle += _update_time_ns;
}

void TimerManager::start_render_timer()
{
	GetRenderTimer().Start();
	_prev_render_start_time = _curr_render_start_time;
	_curr_render_start_time = steady_clock::now();

	_time_until_next_render_ns += _tgt_render_interval_ns;
}

void TimerManager::end_render_timer()
{
	GetRenderTimer().Stop();
	_render_time_ns = steady_clock::now() - _curr_render_start_time;

	auto render_interval = _curr_render_start_time - _prev_render_start_time;

	_avg_render_interval_ns -= _avg_render_interval_ns / _number_of_samples_to_use;
	_avg_render_interval_ns += render_interval / _number_of_samples_to_use;

	//_avg_render_length_ns -= _avg_render_length_ns / _number_of_samples_to_use;
	//_avg_render_length_ns += _render_time_ns / _number_of_samples_to_use;

	_avg_render_length_ns = _render_time_ns;

	_render_this_idle += _render_time_ns;

}

void TimerManager::start_command_list_timer()
{
	_command_list_start_time = steady_clock::now();
}

void TimerManager::end_command_list_timer()
{
	_command_list_interval = steady_clock::now() - _curr_render_start_time;
}

void TimerManager::start_new_idle_interval()
{
	// Push the current idle start _time_manager to previous
	// and Record the start of this idle frame
	_prev_idle_start_time = _curr_idle_start_time;
	_curr_idle_start_time = steady_clock::now();

	// Calculate the actual amount of _time_manager spent idle this frame.
	_idle_time_ns = (_curr_idle_start_time - _prev_idle_start_time) - _update_time_ns - _render_time_ns;

	// Update the rolling sample by removing an average sample 
	// and adding a sample for this frame
	_avg_idle_length_ns -= _avg_idle_length_ns / _number_of_samples_to_use;
	_avg_idle_length_ns += _idle_time_ns / _number_of_samples_to_use;

	// Add the idle _time_manager interval to _time_manager until next, effectively reseting idle _time_manager this frame
	// while still accounting for any _time_manager we went over frame
	// This is used to keep timing inaccuracies from stacking into the math
	_time_until_next_frame_ns += _idle_time_interval_ns;

	// reset update and render _time_manager
	_update_this_idle = 0ns;
	_render_this_idle = 0ns;

}


