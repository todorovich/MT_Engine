// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

namespace mt
{
class Chronometer
{

	static __int64 _next_timer_id;

public:

	friend class TimeManager;

	// Big 6

	Chronometer(char* timer_name, bool can_pause = true)
		: _start_time(0ns)
		, _stop_time(0ns)
		, _time_paused(0ns)
		, _time_continued(0ns)
		, _duration_paused(0ns)
		, _duration_active(0ns)
		, _duration_since_started(0ns)
		, _average_sample_duration(0ns)
		, _name(timer_name)
		, _samples(nullptr)
		, _current_index(0)
		, _timer_ID(-1)
		, _sample_size(100)
		, _can_pause(can_pause)
		, _is_paused(false)
	{
		// Atomic Compare and Swap
		_timer_ID = _next_timer_id;
		_next_timer_id++;
		_samples = new Duration[_sample_size];

		for (auto x = 0; x < _sample_size; x++)
		{
			_samples[x] = 0ns;
		}
	}

	~Chronometer()
	{
		if (_samples != nullptr)
		{
			delete[] _samples;
			_samples = nullptr;
		}
	};

	Chronometer(const Chronometer& other) = delete;
		
	Chronometer(Chronometer&& other)
		: _start_time				(std::move(other._start_time))
		, _stop_time				(std::move(other._stop_time))
		, _time_paused				(std::move(other._time_paused))
		, _time_continued			(std::move(other._time_continued))
		, _duration_paused			(std::move(other._duration_paused))
		, _duration_active			(std::move(other._duration_active))
		, _duration_since_started	(std::move(other._duration_since_started))
		, _average_sample_duration	(std::move(other._average_sample_duration))
		, _name						(std::move(other._name))
		, _samples					(std::move(other._samples))
		, _current_index			(std::move(other._current_index))
		, _timer_ID					(std::move(other._timer_ID))
		, _sample_size				(std::move(other._sample_size))
		, _can_pause				(std::move(other._can_pause))
		, _is_paused				(std::move(other._is_paused))
	{

	}

	Chronometer& operator=(const Chronometer& other) = delete;

	Chronometer& operator=(Chronometer&& other)
	{
		_start_time				 = std::move(other._start_time);
		_stop_time				 = std::move(other._stop_time);
		_time_paused			 = std::move(other._time_paused);
		_time_continued			 = std::move(other._time_continued);
		_duration_paused		 = std::move(other._duration_paused);
		_duration_active		 = std::move(other._duration_active);
		_duration_since_started	 = std::move(other._duration_since_started);
		_average_sample_duration = std::move(other._average_sample_duration);
		_name					 = std::move(other._name);
		_samples				 = std::move(other._samples);
		_current_index			 = std::move(other._current_index);
		_timer_ID				 = std::move(other._timer_ID);
		_sample_size			 = std::move(other._sample_size);
		_can_pause				 = std::move(other._can_pause);
		_is_paused				 = std::move(other._is_paused);
	}

	// Comparison

	friend bool operator<(const Chronometer& lhs, const Chronometer& rhs)
	{
		return lhs._timer_ID < rhs._timer_ID;
	}

	// Methods

	void Start();

	Duration Stop();

	void ResetTimer();

	TimePoint Pause(TimePoint time_paused = Clock::now());

	void Continue(TimePoint time_continued = Clock::now());

	void SetNumberOfSamples(int samples);

	Duration GetLastSample() const;

	Duration GetAverageSampleDuration() const;

	Duration GetDurationActive() const;

	Duration GetDurationPaused() const;

	Duration GetTotalDurationRunning() const;

	std::string& GetName() { return _name; };

private:

	void Tick(const TimePoint& current_tick_time, const TimePoint& previous_tick_time, const Duration& delta_time);
	void _CollectSample();

	TimePoint _start_time;
	TimePoint _stop_time;

	TimePoint _time_paused;
	TimePoint _time_continued;

	Duration _duration_since_started;
	Duration _duration_active;
	Duration _duration_paused;

	Duration _average_sample_duration;

	std::string	_name;
	Duration*	_samples;

	__int64 _timer_ID;
	int _current_index;
	int _sample_size;

	bool _can_pause;
	bool _is_paused;
	bool _is_running;
};
}