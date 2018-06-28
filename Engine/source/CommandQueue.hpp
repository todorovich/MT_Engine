#pragma once

#include "precompiled.hpp"

namespace mt
{
	class CommandQueue
	{
	public:
		CommandQueue() = default;

		~CommandQueue() = default;

		CommandQueue(const CommandQueue& other) = default;

		CommandQueue(CommandQueue&& other) = default;

		CommandQueue& operator=(const CommandQueue& other) = default;

		CommandQueue& operator=(CommandQueue&& other) = default;

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