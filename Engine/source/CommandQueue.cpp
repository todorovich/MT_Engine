#include "precompiled.hpp"

#include "CommandQueue.hpp"

#include "Command.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace mt;

Status CommandQueue::execute_commands()
{
	for (auto i = 0; i < command_queue.size(); i++)
	{
		auto command = command_queue.front();

		command->execute();
		command->destroy();

		command_queue.pop();
	}

	return Status::success;
}
