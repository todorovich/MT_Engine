#include "Chronometer.hpp"

#include "Engine.hpp"
#include "TimeManager.hpp"

using namespace mt;

__int64 Chronometer::_next_timer_id = 1;

void Chronometer::ResetTimer()
{
	_current_index = 0;

	for (auto i = 0; i < _sample_size; i++)
	{
		_samples[i] = 0ns;
	}

	_is_running = false;
	_is_paused = false;

	_start_time		= TimePoint();
	_stop_time		= TimePoint();
	_time_paused	= TimePoint();
	_time_continued = TimePoint();
}

void Chronometer::Start(TimePoint start_time)
{
	if (!_is_running)
	{
		_start_time = start_time;

		_duration_since_started = Duration();
		_duration_active = Duration();
		_total_duration_paused = Duration();
	
		_is_running = true;
	}
}

Duration Chronometer::Stop(TimePoint stop_time)
{
	if (_is_running)
	{
		_is_running = false;

		_stop_time = stop_time;
		
		_duration_since_started = _stop_time - _start_time;
		
		if (_is_paused)
		{
			_duration_paused += _stop_time - Engine::GetTimeManager().GetCurrentTickTime();
		}
		else
		{
			_duration_active += _stop_time - Engine::GetTimeManager().GetCurrentTickTime();
		}

		_CollectSample();

		return _duration_active;
	}
	else
	{
		return 0ns;
	}
}

TimePoint Chronometer::Pause(TimePoint time_paused)
{
	if (_can_pause && _is_running && !_is_paused)
	{
		_is_paused = true;

		_time_paused = time_paused;
	}

	return time_paused;
}

void Chronometer::Continue(TimePoint time_continued)
{
	if (_can_pause && _is_running && _is_paused)
	{
		_time_continued = time_continued;

		_is_paused = false;
	}
}

// PRIVATE FUNCTIONS

void Chronometer::Tick(const TimePoint& current_tick_time, const TimePoint& previous_tick_time, const Duration& delta_time)
{
	if (_is_running)
	{
		_duration_since_started = current_tick_time - _start_time;

		if (_is_paused)
		{
			// PAUSED
			if (previous_tick_time <= _time_paused)
			{
				_duration_active += _time_paused - previous_tick_time;
				_duration_paused = current_tick_time - _time_paused;
			}
			// FULL PAUSE
			else
			{
				_duration_paused += delta_time;
			}
		}
		// Not Paused
		else
		{
			// STARTED
			if (previous_tick_time < _start_time)
			{
				_duration_active += current_tick_time - _start_time;
			}

			else
			{
				// CONTINUED
				if (_time_continued.time_since_epoch() > 0ns && previous_tick_time < _time_continued)
				{
					_total_duration_paused += _time_continued - _time_paused;
					_duration_paused = 0ns;
					_duration_active += current_tick_time - _time_continued;

					_time_paused = TimePoint();
					_time_continued = TimePoint();
				}
				// ACTIVE
				else
				{
					_duration_active += delta_time;
				}
			}
		}
	}
	else
	{
		if (_stop_time > _start_time)
		{
			// STOPPED / INACTIVE
			if (_stop_time.time_since_epoch() > 0ns)
			{

			}
		}
		// NEVER STARTED
		else
		{

		}
	}
}

void Chronometer::_CollectSample()
{
	_average_sample_duration -= _samples[_current_index] / _sample_size;

	_samples[_current_index] = GetTotalDurationRunning();

	_average_sample_duration += _samples[_current_index] / _sample_size;

	_current_index++;
	_current_index %= _sample_size;
}

// ACCESSORS

void Chronometer::SetNumberOfSamples(int new_sample_size)
{	
	Duration* _temp_sample = new Duration[_sample_size];

	auto previous_size = sizeof(Duration) * _sample_size;

	auto new_size = sizeof(Duration) * new_sample_size;

	memcpy_s(_temp_sample, sizeof(Duration) * _sample_size, _samples, sizeof(Duration));

	std::swap(_temp_sample, _samples);

	delete[] _temp_sample;
}

Duration Chronometer::GetLastSample() const
{
	return (_current_index > 0) ? _samples[(_current_index - 1) % _sample_size] : 0ns;
}

Duration Chronometer::GetDurationPaused() const
{
	return _total_duration_paused + _duration_paused;
}

Duration Chronometer::GetAverageSampleDuration() const
{
	return _average_sample_duration;
}

Duration Chronometer::GetDurationActive() const
{
	return _duration_active;
}

Duration Chronometer::GetTotalDurationRunning() const
{
	return _duration_since_started;
}
