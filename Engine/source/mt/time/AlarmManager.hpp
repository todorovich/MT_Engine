#pragma once

#include "mt/precompiled.hpp"
#include "Alarm.hpp"
#include "mt/ObjectPool.hpp"

namespace mt::time
{
	class AlarmManager
	{
	private:

		struct AlarmCompare
		{
			bool operator()(Alarm * alarm_1, Alarm * alarm_2)
			{
				return *alarm_2 < *alarm_1;
			}
		};

		ObjectPool<Alarm, 1024>				_alarm_pool;

		std::priority_queue <Alarm*, std::vector <Alarm*>, AlarmCompare>	_alarm_queue;

		std::set <Alarm*>													_alarms_and_timers;

	public:
		AlarmManager() = default;

		~AlarmManager() = default;

		AlarmManager(const AlarmManager& other) = delete;

		AlarmManager(AlarmManager&& other) = delete;

		AlarmManager& operator=(const AlarmManager& other) = delete;

		AlarmManager& operator=(AlarmManager&& other) = delete;

		void Tick(TimePoint current_tick_time, TimePoint previous_tick_time, Duration delta_time);

		void Pause(TimePoint time_paused = Clock::now());

		void Continue(TimePoint time_continued = Clock::now());

		void AddAlarm(TimePoint time_point, std::function<void()> function, bool repeats = false, Duration repeat_interval = 0ns)
		{
			Alarm* alarm = new (_alarm_pool.GetMemory()) Alarm(time_point, function, repeats, repeat_interval);

			_alarm_queue.push(alarm);
			_alarms_and_timers.insert(alarm);
		}

	};
}