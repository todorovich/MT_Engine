// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

namespace mt
{
	enum class EXECUTION_TYPE
	{
		IMMEDIATE,		// CREATOR MAY NOT CALL EXECUTE OR DESTROY, EXECUTION IS IMMEDIATE
		ASYNCHRONOURS,	// CREATOR MAY NOT CALL EXECUTE OR DESTROY, EXECUTION IS ASYNCHRONOUS
		UNMANAGED,		// CREATER MUST CALL EXECUTE AND DESTROY
	};

	class Command
	{
		friend CommandQueue;

	public:
		Command(EXECUTION_TYPE method_of_execution = EXECUTION_TYPE::ASYNCHRONOURS)
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
		};

		virtual void execute() = 0;

		void Destroy()
		{
			delete this;
		};

	protected:

		virtual void add_to_command_queue();

		virtual ~Command() = default;

		Command(const Command& other) = default;

		Command(Command&& other) = default;

		Command& operator=(const Command& other) = default;

		Command& operator= (Command&& other) = default;

	};
}