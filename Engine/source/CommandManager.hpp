// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

namespace mt
{
	class CommandManager
	{
	public:
		CommandManager() = default;

		~CommandManager() = default;

		CommandManager(const CommandManager& other) = default;

		CommandManager(CommandManager&& other) = default;

		CommandManager& operator=(const CommandManager& other) = default;

		CommandManager& operator=(CommandManager&& other) = default;

		Status add_command(Command* const command_to_add)
		{
			command_queue.push(command_to_add);

			return Status::success;
		}

		Status execute_commands();

	private:
		std::queue<Command*> command_queue;

	};
}