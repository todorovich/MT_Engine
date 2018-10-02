// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

using steady_clock	= std::chrono::steady_clock;
using period		= std::chrono::steady_clock::period;
using duration		= std::chrono::steady_clock::duration;
using time_point	= std::chrono::steady_clock::time_point;

using namespace std::literals::chrono_literals;

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
		_samples = new duration[_sample_size];

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
		, _average_sample_duration			(std::move(other._average_sample_duration))
		, _name						(std::move(other._name))
		, _samples					(std::move(other._samples))
		, _current_index			(std::move(other._current_index))
		, _timer_ID					(std::move(other._timer_ID))
		, _sample_size				(std::move(other._sample_size))
		, _can_pause				(std::move(other._can_pause))
		, _is_paused			(std::move(other._is_paused))
	{

	}

	Chronometer&& operator=(const Chronometer& other) = delete;

	Chronometer&& operator=(Chronometer&& other)
	{
		_start_time				= std::move(other._start_time);
		_stop_time				= std::move(other._stop_time);
		_time_paused			= std::move(other._time_paused);
		_time_continued			= std::move(other._time_continued);
		_duration_paused		= std::move(other._duration_paused);
		_duration_active		= std::move(other._duration_active);
		_duration_since_started	= std::move(other._duration_since_started);
		_average_sample_duration		= std::move(other._average_sample_duration);
		_name					= std::move(other._name);
		_samples				= std::move(other._samples);
		_current_index			= std::move(other._current_index);
		_timer_ID				= std::move(other._timer_ID);
		_sample_size			= std::move(other._sample_size);
		_can_pause				= std::move(other._can_pause);
		_is_paused		= std::move(other._is_paused);
	}

	// Comparison

	friend bool operator<(const Chronometer& lhs, const Chronometer& rhs)
	{
		return lhs._timer_ID < rhs._timer_ID;
	}

	// Methods

	void Start();

	duration Stop();

	void ResetTimer();

	time_point Pause();

	void Continue();

	void SetNumberOfSamples(int samples);

	duration GetLastSample() const;

	duration GetAverageSampleDuration() const;

	duration GetDurationActive() const;

	duration GetDurationPaused() const;

	duration GetTotalDurationRunning() const;

	std::string& GetName() { return _name; };

private:

	void _Tick(const time_point& current_tick_time, const time_point& previous_tick_time, const duration& delta_time);
	void _CollectSample();


	time_point _Pause(time_point time_paused);
	void _Continue(time_point time_continued);

	time_point _start_time;
	time_point _stop_time;

	time_point _time_paused;
	time_point _time_continued;

	duration _duration_since_started;
	duration _duration_active;
	duration _duration_paused;

	duration _average_sample_duration;

	std::string	_name;
	duration*	_samples;

	__int64 _timer_ID;
	int _current_index;
	int _sample_size;

	bool _can_pause;
	bool _is_paused;
	bool _is_running;
};
}