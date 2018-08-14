// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

#include "Log.hpp"

class LogManager
{
	// Default Constructor
	LogManager() = default;

	// Copy Constructor
	LogManager(const LogManager& other) = delete;

	// Move Constructor
	LogManager(LogManager&& other) = default;

	// Copy Assignment
	LogManager& operator=(const LogManager& other) = delete;

	// Move Assignment
	LogManager& operator=(LogManager&& other) = default;

	// Destructor
	~LogManager() = default;
};