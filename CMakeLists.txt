cmake_minimum_required(VERSION 3.16)
project(mt_engine VERSION 0.1 DESCRIPTION "The mt engine" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if (MSVC)
	add_compile_options(/Zi /EHcs)
else()
	add_compile_options(-fexceptions)
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_definitions(-DUNICODE -D_UNICODE)

set(ignoreMe "${CMAKE_C_COMPILER}${CMAKE_C_FLAGS}")


#include_directories("/mnt/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include")
#include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um")
#include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt")
#include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared")

link_directories("c:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64")


add_library(luna STATIC
	Engine/source/luna/DDSTextureLoader12.cpp
	Engine/source/luna/DDSTextureLoader12.h
	Engine/source/luna/d3dUtil.cpp
	Engine/source/luna/d3dUtil.h
	Engine/source/luna/d3dx12.h
	Engine/source/luna/GeometryGenerator.cpp
	Engine/source/luna/GeometryGenerator.h
	Engine/source/luna/MathHelper.cpp
	Engine/source/luna/MathHelper.h
	Engine/source/luna/UploadBuffer.h
)

add_executable(mt_engine WIN32
		Engine/source/mt/time/Alarm.cpp
		Engine/source/mt/time/Alarm.hpp
		Engine/source/mt/time/AlarmManager.cpp
		Engine/source/mt/time/AlarmManager.hpp
		Engine/source/mt/io/Archiver.cpp
		Engine/source/mt/io/Archiver.hpp
		Engine/source/mt/camera/Camera.cpp
		Engine/source/mt/camera/Camera.hpp
		Engine/source/mt/time/Chronometer.cpp
		Engine/source/mt/time/Chronometer.hpp
		Engine/source/mt/command/Command.cpp
		Engine/source/mt/command/Command.hpp
		Engine/source/mt/command/CommandManager.cpp
		Engine/source/mt/command/CommandManager.hpp
		Engine/source/mt/renderer/DirectXRenderer.cpp
		Engine/source/mt/renderer/DirectXRenderer.hpp
		Engine/source/mt/Engine.cpp
		Engine/source/mt/Engine.hpp
		Engine/source/mt/input/IM_KeyDown.hpp
		Engine/source/mt/input/IM_KeyUp.hpp
		Engine/source/mt/input/IM_MouseDown.hpp
		Engine/source/mt/input/IM_MouseMove.hpp
		Engine/source/mt/input/IM_MouseUp.hpp
		Engine/source/mt/input/InputManager.cpp
		Engine/source/mt/input/InputManager.hpp
		Engine/source/mt/input/InputMessage.cpp
		Engine/source/mt/input/InputMessage.hpp
		Engine/source/mt/logging/Log.hpp
		Engine/source/mt/logging/LogManager.hpp
		Engine/source/mt/ObjectPool.hpp
		Engine/source/mt/Pool.hpp
		Engine/source/mt/precompiled.cpp
		Engine/source/mt/precompiled.hpp
		Engine/source/mt/time/TimeManager.cpp
		Engine/source/mt/time/TimeManager.hpp
		Engine/source/mt/time/Timer.hpp
		Engine/source/mt/windows/WindowsMessage.cpp
		Engine/source/mt/windows/WindowsMessage.hpp
		Engine/source/mt/windows/WindowsMessageManager.cpp
		Engine/source/mt/windows/WindowsMessageManager.hpp
		Engine/source/mt/windows/WM_Activate.cpp
		Engine/source/mt/windows/WM_Activate.hpp
		Engine/source/mt/windows/WM_CaptureChanged.cpp
		Engine/source/mt/windows/WM_CaptureChanged.hpp
		Engine/source/mt/windows/WM_Close.cpp
		Engine/source/mt/windows/WM_Close.hpp
		Engine/source/mt/windows/WM_Destroy.cpp
		Engine/source/mt/windows/WM_Destroy.hpp
		Engine/source/mt/windows/WM_EnterSizeMove.cpp
		Engine/source/mt/windows/WM_EnterSizeMove.hpp
		Engine/source/mt/windows/WM_EraseBackground.cpp
		Engine/source/mt/windows/WM_EraseBackground.hpp
		Engine/source/mt/windows/WM_ExitSizeMove.cpp
		Engine/source/mt/windows/WM_ExitSizeMove.hpp
		Engine/source/mt/windows/WM_GetIcon.cpp
		Engine/source/mt/windows/WM_GetIcon.hpp
		Engine/source/mt/windows/WM_GetMinMaxInfo.cpp
		Engine/source/mt/windows/WM_GetMinMaxInfo.hpp
		Engine/source/mt/windows/WM_Input.cpp
		Engine/source/mt/windows/WM_Input.hpp
		Engine/source/mt/windows/WM_KeyUp.cpp
		Engine/source/mt/windows/WM_KeyUp.hpp
		Engine/source/mt/windows/WM_LeftMouseButtonDown.cpp
		Engine/source/mt/windows/WM_LeftMouseButtonDown.hpp
		Engine/source/mt/windows/WM_LeftMouseButtonUp.cpp
		Engine/source/mt/windows/WM_LeftMouseButtonUp.hpp
		Engine/source/mt/windows/WM_MenuChar.cpp
		Engine/source/mt/windows/WM_MenuChar.hpp
		Engine/source/mt/windows/WM_MiddleMouseButtonDown.cpp
		Engine/source/mt/windows/WM_MiddleMouseButtonDown.hpp
		Engine/source/mt/windows/WM_MiddleMouseButtonUp.cpp
		Engine/source/mt/windows/WM_MiddleMouseButtonUp.hpp
		Engine/source/mt/windows/WM_MouseMove.cpp
		Engine/source/mt/windows/WM_MouseMove.hpp
		Engine/source/mt/windows/WM_Move.cpp
		Engine/source/mt/windows/WM_Move.hpp
		Engine/source/mt/windows/WM_Moving.cpp
		Engine/source/mt/windows/WM_Moving.hpp
		Engine/source/mt/windows/WM_NonClientCalcSize.cpp
		Engine/source/mt/windows/WM_NonClientCalcSize.hpp
		Engine/source/mt/windows/WM_NonClientMouseLeave.cpp
		Engine/source/mt/windows/WM_NonClientMouseLeave.hpp
		Engine/source/mt/windows/WM_NonClientPaint.cpp
		Engine/source/mt/windows/WM_NonClientPaint.hpp
		Engine/source/mt/windows/WM_Paint.cpp
		Engine/source/mt/windows/WM_Paint.hpp
		Engine/source/mt/windows/WM_Quit.cpp
		Engine/source/mt/windows/WM_Quit.hpp
		Engine/source/mt/windows/WM_RightMouseButtonDown.cpp
		Engine/source/mt/windows/WM_RightMouseButtonDown.hpp
		Engine/source/mt/windows/WM_RightMouseButtonUp.cpp
		Engine/source/mt/windows/WM_RightMouseButtonUp.hpp
		Engine/source/mt/windows/WM_SetText.cpp
		Engine/source/mt/windows/WM_SetText.hpp
		Engine/source/mt/windows/WM_Size.cpp
		Engine/source/mt/windows/WM_Size.hpp
		Engine/source/mt/windows/WM_Sizing.cpp
		Engine/source/mt/windows/WM_Sizing.hpp
		Engine/source/mt/windows/WM_Timer.cpp
		Engine/source/mt/windows/WM_Timer.hpp
		Engine/source/mt/windows/WM_WindowPositionChanging.cpp
		Engine/source/mt/windows/WM_WindowPositionChanging.hpp
		EngineDemo/source/EngineDemo.cpp)

target_include_directories(mt_engine SYSTEM PRIVATE Engine/source/luna)
target_include_directories(mt_engine PUBLIC Engine/source/mt)

target_link_libraries(mt_engine luna)