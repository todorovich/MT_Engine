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

	_AddEngineAlarms();

	curr_tick_time	 = Clock::now();
	prev_tick_time	 = TimePoint(0ns);

	tick_delta_time_ns = 0ns;

	_is_paused = false;
}

void TimeManager::Tick()
{
	prev_tick_time = curr_tick_time;

	curr_tick_time = Clock::now();

	tick_delta_time_ns = curr_tick_time - prev_tick_time;

	_alarm_manager.Tick(curr_tick_time, prev_tick_time, tick_delta_time_ns);

	// Tick all the chronometers
	for (auto& chronometer : _chronometers)
	{
		chronometer.second->Tick(curr_tick_time, prev_tick_time, tick_delta_time_ns);
	}
}

void TimeManager::UpdateComplete() 
{ 
	_should_update = false; 
}

void TimeManager::RenderComplete()
{
	_should_render = false;
}

void TimeManager::Continue()
{
	// If the game is paused, unpause it.
	if (_is_paused == true)
	{
		_is_paused = false;

		auto continue_time = Clock::now();

		_alarm_manager.Continue(continue_time);
		
		for (auto& pair : _chronometers)
		{
			Chronometer*& timer = pair.second;
			timer->Continue(continue_time);
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

		auto time_paused = Clock::now();

		_alarm_manager.Pause(time_paused);

		for (auto& pair : _chronometers)
		{
			Chronometer*& chrono = pair.second;
			chrono->Pause(time_paused);
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

void TimeManager::_AddEngineAlarms()
{
	_alarm_manager.AddAlarm(Clock::now() + _tgt_update_interval_ns, [&]() -> void { _SetShouldUpdate(); }, true , _tgt_update_interval_ns);
	_alarm_manager.AddAlarm(Clock::now() + _tgt_render_interval_ns, [&]() -> void { _SetShouldRender(); }, true, _tgt_render_interval_ns);
}

void TimeManager::_SetShouldUpdate() 
{ 
	_should_update = true;
}

void TimeManager::_SetShouldRender()
{
	_should_render = true;
}