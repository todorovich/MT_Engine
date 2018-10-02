// Copyright 2018 Micho Todorovich, all rights reserved.

#include "TimeManager.hpp"
#include "engine.hpp"

using namespace mt;

const int TimeManager::_number_of_samples_to_use = 100;

void TimeManager::Initialize()
{
	_DeleteAllChronometers();

	_AddEngineChronometers();

	_StartAllChronometers();

	_curr_tick_time	 = steady_clock::now();
	_prev_tick_time	 = time_point(0ns);

	_time_until_next_update_ns = _tgt_update_interval_ns;
	_time_until_next_render_ns = _tgt_render_interval_ns;
	_time_until_next_frame_ns = _idle_time_interval_ns;

	_tick_delta_time_ns = 0ns;

	_is_paused = false;
}

void TimeManager::Tick()
{
	_prev_tick_time = _curr_tick_time;

	_curr_tick_time = steady_clock::now();

	_tick_delta_time_ns = _curr_tick_time - _prev_tick_time;

	// Tick all the timers
	for (auto& chronometer : _chronometers)
	{
		chronometer.second->_Tick(_curr_tick_time, _prev_tick_time, _tick_delta_time_ns);
	}

	_time_until_next_frame_ns -= _tick_delta_time_ns;

	if (!_is_paused)
	{
		_time_until_next_update_ns -= _tick_delta_time_ns;
	}

	if (!_is_render_paused)
	{
		_time_until_next_render_ns -= _tick_delta_time_ns;		
	}
}

void TimeManager::Continue()
{
	// If the game is paused, unpause it.
	if (_is_paused == true)
	{
		_is_paused = false;

		auto continue_time = steady_clock::now();

		for (auto& pair : _chronometers)
		{
			Chronometer*& timer = pair.second;
			timer->_Continue(continue_time);
		}
	}

	// else do nothing
}

void TimeManager::Pause()
{
	// If the game is not paused, pause it.
	if(_is_paused == false)
	{
		_is_paused = true;

		auto time_paused = steady_clock::now();

		for (auto& pair : _chronometers)
		{
			Chronometer*& timer = pair.second;
			timer->_Pause(time_paused);
		}
	}

	// else do nothing
}

void TimeManager::_DeleteAllChronometers()
{
	for (auto& pair : _chronometers)
	{
		Chronometer*& timer = pair.second;
		delete timer;
	}

	_chronometers.clear();
}

void TimeManager::_AddEngineChronometers()
{
	auto chronometer = new Chronometer("Total Up-Time");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Update Chonometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Render Chonometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Statistics Chonometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Frame Chonometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));
	
	chronometer = new Chronometer("Windows Message Chronometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Input Chronometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("Tick Chonometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

	chronometer = new Chronometer("In Between Ticks Chonometer");

	_chronometers.insert(std::make_pair(chronometer->GetName(), chronometer));

}

void TimeManager::_StartAllChronometers()
{
	for (auto& pair : _chronometers)
	{
		Chronometer*& timer = pair.second;
		timer->Start();
	}
}