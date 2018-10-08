#include "AlarmManager.hpp"

using namespace mt;

void AlarmManager::Tick(TimePoint current_tick_time, TimePoint previous_tick_time, Duration delta_time)
{
	Alarm* alarm = _alarm_queue.top();

	alarm->Tick(current_tick_time, previous_tick_time, delta_time);

	while (alarm->HasTriggered())
	{
		_alarm_queue.pop();

		if (alarm->_alarm_repeats == true)
		{
			alarm->_alarm_time += alarm->_reset_interval;
			alarm->_has_triggered = false;
			_alarm_queue.push(alarm);
		}
		else
		{
			_alarm_pool.ReleaseMemory(alarm);
			_alarms_and_timers.erase(alarm);
		}

		alarm = _alarm_queue.top();

		alarm->Tick(current_tick_time, previous_tick_time, delta_time);
	}

}

void AlarmManager::Pause(TimePoint time_paused)
{
	for (auto& alarm : _alarms_and_timers)
	{
		alarm->Pause(time_paused);
	}
}

void AlarmManager::Continue(TimePoint time_continued)
{
	for (auto& alarm : _alarms_and_timers)
	{
		alarm->Continue(time_continued);
	}
}
