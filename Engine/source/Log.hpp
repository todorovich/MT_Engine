// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

class Log
{



public:

	// Default Constructor
	Log() = default;
	
	// Copy Constructor
	Log(const Log& other) = delete;

	// Move Constructor
	Log(Log&& other) = default;

	// Copy Assignment
	Log& operator=(const Log& other) = delete;

	// Move Assignment
	Log& operator=(Log&& other) = default;

	// Destructor
	~Log() = default;


};