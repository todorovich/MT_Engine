
#include "precompiled.hpp"

#include "Command.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

void Command::add_to_command_queue()
{
	engine::GetCommandManager().add_command(this);
}
