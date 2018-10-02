// Copywrite

#pragma once

// Monolithic Precompiled Header.
// This is, in general, only for std libraries that don't change much.

#include <algorithm>
#include <array>					// Standard Array Container
#include <assert.h>
#include <atomic>
#include <cassert>
#include <chrono>
#include <comdef.h>
#include <cmath>
#include <cstdarg>
#include <cstdint>					// Standard Fixed Integer types
#include <deque>					// Standard Double Ended Queue
#include <d3d12.h>					// DirectX 12 library
#include <d3dcompiler.h>
#include <DirectXMath.h>			// DirectX Math Library
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#include <dxgi1_4.h>
#include <iostream>
#include <filesystem>
#include <float.h>
#include <fstream>					// Filestream
#include <list>						// Standard List Conatainer
#include <map>						// Standard Map Conatainer
#include <memory>
#include <mutex>
#include <queue>					// Standard Queue Conatiner
#include <ratio>
#include <set>
#include <sstream>					// String Stream
#include <streambuf>
#include <string>					// String
#include <thread>
#include <type_traits>
#include <tuple>					// Standard Tuple
#include <thread>
#include <unordered_map>			// Standard Unordered Map (hash map) Container
#include <utility>
#include <vector>					// Standard Vector Container
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
namespace mt
{
	void SetThreadName(DWORD dwThreadID, const char* threadName);

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
