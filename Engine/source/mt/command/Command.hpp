// Copyright 2018 Micho Todorovich, all rights reserved.

#pragma once

#include "precompiled.hpp"

namespace mt::command
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
		explicit Command(EXECUTION_TYPE method_of_execution = EXECUTION_TYPE::ASYNCHRONOURS) ;

		virtual void execute() {};

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