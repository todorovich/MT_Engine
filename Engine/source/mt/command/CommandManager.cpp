// Copyright 2018 Micho Todorovich, all rights reserved.

#include "CommandManager.hpp"

#include "Command.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace mt::command;

mt::Status CommandManager::execute_commands()
{
	for (auto i = 0; i < command_queue.size(); i++)
	{
		auto command = command_queue.front();

		command->execute();
		command->Destroy();

		command_queue.pop();
	}

	return Status::success;
}
