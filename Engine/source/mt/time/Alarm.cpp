#include "Alarm.hpp"

using namespace mt::time;

void Alarm::Tick(TimePoint current_tick_time, __attribute__((unused))TimePoint previous_tick_time, __attribute__((unused))Duration delta_time)
{
	// Not triggered or paused
	if (_has_triggered == false && _is_paused == false)
	{
		if (current_tick_time >= _alarm_time)
		{
			_has_triggered = true;

			_callback();
		}
	}
}

void Alarm::Pause(TimePoint time_paused)
{
	_time_paused = time_paused;
	_is_paused = true;
}

void Alarm::Continue(TimePoint time_continued)
{
	// Offset time time by the amount of time spent paused;
	_alarm_time += time_continued - _time_paused;

	_time_paused = TimePoint(0ns);

	_is_paused = false;
}
