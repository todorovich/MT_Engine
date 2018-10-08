// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"
#include "AlarmManager.hpp"
#include "Chronometer.hpp"
#include "Timer.hpp"

namespace mt
{
	class TimeManager
	{
		static const int		_number_of_samples_to_use;
	
	public:
		TimeManager()
			: _tgt_update_interval_ns(8333333ns)
			, _tgt_render_interval_ns(16666666ns)
			, _idle_time_interval_ns (16666666ns)
			, _command_list_interval (0ns)
			, _is_paused(true)
		{}

		~TimeManager()
		{
			_DeleteAllChronometers();
		}
	
		TimePoint GetCurrentTickTime() const { return curr_tick_time; };

		TimePoint GetPreviousTickTime() const { return prev_tick_time; };

		Duration GetTickDeltaTime() const { return tick_delta_time_ns; };

		Duration DurationSinceLaunch() const { return _chronometers.find(std::string("Total Game Time"))->second->GetTotalDurationRunning(); };

		Duration GetTargetUpdateInterval() const { return _tgt_update_interval_ns; }

		Duration GetTargetRenderInterval() const { return _tgt_render_interval_ns; }

		bool GetShouldUpdate() const { return _should_update; }
		bool GetShouldRender() const { return _should_render; }

		void Initialize();		// Call before message loop.
		void Continue();		// Call to unpaused.
		void Pause();			// Call to pause.
		void Tick();			// Call every frame.
	
		void UpdateComplete();
		void RenderComplete();

		Chronometer& FindTimer(std::string name) 
		{ 
			auto find = _chronometers.find(name);
			if (find == _chronometers.end())
			{
				throw;
			}
			else
			{
				return *find->second;	
			}
		}

		Chronometer&  GetGameTimer() const { return *(_chronometers.find(std::string("Total Up-Time"))->second); }
		Chronometer&  GetUpdateTimer() const { return *(_chronometers.find(std::string("Update Chonometer"))->second); }
		Chronometer&  GetRenderTimer() const { return *(_chronometers.find(std::string("Render Chonometer"))->second); }
		Chronometer&  GetStatisticsTimer() const { return *(_chronometers.find(std::string("Statistics Chonometer"))->second); }
		Chronometer&  GetTickTimer() const { return *(_chronometers.find(std::string("Tick Chonometer"))->second); }
		
		bool IsUpdatePaused() const { return _is_paused; }
		//bool IsRenderPaused() const { return _is_render_paused; }
	
	private:
		void _DeleteAllChronometers();
		void _AddEngineChronometers();
		void _StartAllChronometers();
		void _AddEngineAlarms();
		
		void _SetShouldUpdate();
		void _SetShouldRender();
		
		AlarmManager							_alarm_manager;
		std::map<std::string, Chronometer*>		_chronometers;

		Duration	_tgt_update_interval_ns;
		Duration	_tgt_render_interval_ns;
		Duration	_idle_time_interval_ns;
		Duration	_command_list_interval;
	
		TimePoint curr_tick_time = TimePoint(0ns);
		TimePoint prev_tick_time = TimePoint(0ns);
	
		Duration tick_delta_time_ns	 = 0ns;

		bool _is_paused;

		bool _should_update;
		bool _should_render;

	};
}