
#include "precompiled.hpp"

#include "Command.hpp"

#include "Engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt::command;

void Command::add_to_command_queue()
{
	Engine::GetCommandManager().add_command(this);
}

Command::Command(EXECUTION_TYPE method_of_execution)
{
    switch (method_of_execution)
    {
        case EXECUTION_TYPE::IMMEDIATE:
            execute();
            Destroy();
            break;
        case EXECUTION_TYPE::ASYNCHRONOURS:
            add_to_command_queue();
            break;
        case EXECUTION_TYPE::UNMANAGED:
            break;
    }
}
