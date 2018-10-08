#pragma once

#include "precompiled.hpp"

namespace mt
{
	class Alarm
	{
	private:
		TimePoint _alarm_time;

		TimePoint _time_paused;

		std::function<void()> _callback;

		Duration _reset_interval;

		bool _alarm_repeats;

		bool _has_triggered;

		bool _is_paused;

		void Tick(TimePoint current_tick_time, TimePoint previous_tick_time, Duration delta_time);

	public:

		friend class AlarmManager;

		Alarm(TimePoint time_point = TimePoint(), std::function<void()> callback = []() {}, bool alarm_repeats = false, Duration reset_interval = 0ns)
			: _alarm_time(time_point)
			, _time_paused(TimePoint(0ns))
			, _callback(callback)
			, _reset_interval(reset_interval)
			, _alarm_repeats(alarm_repeats)
			, _is_paused(false)
		{
			
		}
			
		~Alarm() = default;

		Alarm(const Alarm& other) = delete;
		
		Alarm(Alarm&& other)
			: _alarm_time(std::move(other._alarm_time))
			, _callback(std::move(other._callback))
		{};

		Alarm& operator=(const Alarm& other) = delete;

		Alarm& operator=(Alarm&& other)	
		{
			_alarm_time = std::move(other._alarm_time);
			_callback = std::move(other._callback);

			return *this;
		};

		bool operator<(const Alarm& other) const
		{
			return _alarm_time < other._alarm_time;
		}

		bool HasTriggered() 
		{ 
			return _has_triggered; 
		}

		void Pause(TimePoint time_paused = Clock::now());

		void Continue(TimePoint time_continued = Clock::now());
	};
}