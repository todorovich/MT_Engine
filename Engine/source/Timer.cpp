#include "Timer.hpp"

#include "Engine.hpp"
#include "TimerManager.hpp"

using namespace mt;

__int64 Timer::_next_timer_id = 1;

void Timer::ResetTimer()
{
	_current_index = 0;

	for (auto i = 0; i < _sample_size; i++)
	{
		_samples[i] = 0ns;
	}

	_is_running = false;
	_is_paused = false;

	_start_time		= time_point();
	_stop_time		= time_point();
	_time_paused	= time_point();
	_time_continued = time_point();
}

void Timer::Start()
{
	if (!_is_running)
	{
		_start_time = steady_clock::now();

		_duration_since_started = duration();
		_duration_active = duration();
		_duration_paused = duration();
	
		_is_running = true;
	}
}

duration Timer::Stop()
{
	if (_is_running)
	{
		_stop_time = steady_clock::now();

		_is_running = false;
		
		_duration_since_started = _stop_time - _start_time;
		
		_duration_active += _stop_time - Engine::GetTimerManager().GetCurrentTickTime();

		_CollectSample();

		return _duration_active;
	}
	else
	{
		return 0ns;
	}
}

time_point Timer::Pause()
{
	return _Pause(steady_clock::now());
}

void Timer::Continue()
{
	_Continue(steady_clock::now());
}

// PRIVATE FUNCTIONS

time_point Timer::_Pause(time_point time_paused)
{
	if (_can_pause && _is_running &&!_is_paused)
	{
		_is_paused = true;

		_time_paused = time_paused;
	}

	return time_paused;
}

void Timer::_Continue(time_point time_continued)
{
	if (_can_pause && _is_running && _is_paused)
	{
		_time_continued = time_continued;

		_is_paused = false;
	}
}

void Timer::_Tick(const time_point& current_tick_time, const time_point& previous_tick_time, const duration& delta_time)
{
	if (_is_running)
	{
		_duration_since_started = current_tick_time - _start_time;

		if (_is_paused)
		{
			// PAUSED
			if (previous_tick_time < _time_paused)
			{
				_duration_active += _time_paused - previous_tick_time;
				_duration_paused += current_tick_time - _time_paused;
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
					_duration_paused += _time_continued - previous_tick_time;
					_duration_active += current_tick_time - _time_continued;

					_time_paused = time_point();
					_time_continued = time_point();
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

void Timer::_CollectSample()
{
	_average_sample_duration -= _samples[_current_index] / _sample_size;

	_samples[_current_index] = GetTotalDurationRunning();

	_average_sample_duration += _samples[_current_index] / _sample_size;

	_current_index++;
	_current_index %= _sample_size;
}

// ACCESSORS

void Timer::SetNumberOfSamples(int new_sample_size)
{	
	duration* _temp_sample = new duration[_sample_size];

	auto previous_size = sizeof(duration) * _sample_size;

	auto new_size = sizeof(duration) * new_sample_size;

	memcpy_s(_temp_sample, sizeof(duration) * _sample_size, _samples, sizeof(duration));

	std::swap(_temp_sample, _samples);

	delete[] _temp_sample;
}

duration Timer::GetLastSample() const
{
	return _samples[(_current_index - 1) % _sample_size];
}

duration Timer::GetDurationPaused() const
{
	return _duration_paused;
}

duration Timer::GetAverageSampleDuration() const
{
	return _average_sample_duration;
}

duration Timer::GetDurationActive() const
{
	return _duration_active;
}

duration Timer::GetTotalDurationRunning() const
{
	return _duration_since_started;
}
