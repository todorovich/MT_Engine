#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdarg>
#include <deque>
#include <iostream>
#include <filesystem>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <ratio>
#include <set>
#include <string>
#include <thread>
#include <type_traits>
#include <tuple>
#include <utility>
#include <windows.h>

#ifdef _DEBUG
	#include <stdlib.h>
	#include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
	#define DEBUG_NEW new
#endif

#include "MathHelper.h"

namespace mt
{
	enum class Status
	{
		incomplete = -2,
		failure = -1,
		negative = false, // 0
		affirmative = true,  // 1
		success = 2,
	};

	class Command;
	class CommandQueue;

}
