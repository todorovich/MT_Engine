cmake_minimum_required(VERSION 3.16)
project(mt_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(MSVC_COMPILE_FLAGS "/MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_COMPILE_FLAGS}" )

if (MSVC)
    # warning level 4 and all warnings as errors
    # /W4 /WX
    add_compile_options(/W0 /GX /MP /Od /Zi)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -fdeclspec -fexceptions)
endif()

add_definitions(-DUNICODE -D_UNICODE)

include_directories(Engine/source)
include_directories("/mnt/c/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include")
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um")
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt")
include_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared")

link_directories("/mnt/c/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64")

add_executable(mt_engine WIN32
		Engine/source/mt/time/Alarm.cpp
		Engine/source/mt/time/Alarm.hpp
		Engine/source/mt/time/AlarmManager.cpp
		Engine/source/mt/time/AlarmManager.hpp
		Engine/source/mt/Archiver.cpp
		Engine/source/mt/Archiver.hpp
		Engine/source/mt/Camera.cpp
		Engine/source/mt/Camera.hpp
		Engine/source/mt/time/Chronometer.cpp
		Engine/source/mt/time/Chronometer.hpp
		Engine/source/mt/Command.cpp
		Engine/source/mt/Command.hpp
		Engine/source/mt/CommandManager.cpp
		Engine/source/mt/CommandManager.hpp
		Engine/source/d3dUtil.cpp
		Engine/source/d3dUtil.h
		Engine/source/d3dx12.h
		Engine/source/DDSTextureLoader12.cpp
		Engine/source/DDSTextureLoader12.h
		Engine/source/mt/renderer/DirectXRenderer.cpp
		Engine/source/mt/renderer/DirectXRenderer.hpp
		Engine/source/mt/Engine.cpp
		Engine/source/mt/Engine.hpp
		Engine/source/GeometryGenerator.cpp
		Engine/source/GeometryGenerator.h
		Engine/source/mt/IM_KeyDown.hpp
		Engine/source/mt/IM_KeyUp.hpp
		Engine/source/mt/IM_MouseDown.hpp
		Engine/source/mt/IM_MouseMove.hpp
		Engine/source/mt/IM_MouseUp.hpp
		Engine/source/mt/InputManager.cpp
		Engine/source/mt/InputManager.hpp
		Engine/source/mt/InputMessage.cpp
		Engine/source/mt/InputMessage.hpp
		Engine/source/Log.hpp
		Engine/source/LogManager.hpp
		Engine/source/MathHelper.cpp
		Engine/source/MathHelper.h
		Engine/source/mt/ObjectPool.hpp
		Engine/source/mt/Pool.hpp
		Engine/source/precompiled.cpp
		Engine/source/precompiled.hpp
		Engine/source/mt/time/TimeManager.cpp
		Engine/source/mt/time/TimeManager.hpp
		Engine/source/mt/time/Timer.hpp
		Engine/source/UploadBuffer.h
		Engine/source/WindowsMessage.cpp
		Engine/source/WindowsMessage.hpp
		Engine/source/mt/WindowsMessageManager.cpp
		Engine/source/mt/WindowsMessageManager.hpp
		Engine/source/mt/WM_Activate.cpp
		Engine/source/mt/WM_Activate.hpp
		Engine/source/mt/WM_CaptureChanged.cpp
		Engine/source/mt/WM_CaptureChanged.hpp
		Engine/source/mt/WM_Close.cpp
		Engine/source/mt/WM_Close.hpp
		Engine/source/mt/WM_Destroy.cpp
		Engine/source/mt/WM_Destroy.hpp
		Engine/source/mt/WM_EnterSizeMove.cpp
		Engine/source/mt/WM_EnterSizeMove.hpp
		Engine/source/mt/WM_EraseBackground.cpp
		Engine/source/mt/WM_EraseBackground.hpp
		Engine/source/mt/WM_ExitSizeMove.cpp
		Engine/source/mt/WM_ExitSizeMove.hpp
		Engine/source/mt/WM_GetIcon.cpp
		Engine/source/mt/WM_GetIcon.hpp
		Engine/source/mt/WM_GetMinMaxInfo.cpp
		Engine/source/mt/WM_GetMinMaxInfo.hpp
		Engine/source/mt/WM_Input.cpp
		Engine/source/mt/WM_Input.hpp
		Engine/source/mt/WM_KeyUp.cpp
		Engine/source/mt/WM_KeyUp.hpp
		Engine/source/mt/WM_LeftMouseButtonDown.cpp
		Engine/source/mt/WM_LeftMouseButtonDown.hpp
		Engine/source/mt/WM_LeftMouseButtonUp.cpp
		Engine/source/mt/WM_LeftMouseButtonUp.hpp
		Engine/source/mt/WM_MenuChar.cpp
		Engine/source/mt/WM_MenuChar.hpp
		Engine/source/mt/WM_MiddleMouseButtonDown.cpp
		Engine/source/mt/WM_MiddleMouseButtonDown.hpp
		Engine/source/mt/WM_MiddleMouseButtonUp.cpp
		Engine/source/mt/WM_MiddleMouseButtonUp.hpp
		Engine/source/mt/WM_MouseMove.cpp
		Engine/source/mt/WM_MouseMove.hpp
		Engine/source/mt/WM_Move.cpp
		Engine/source/mt/WM_Move.hpp
		Engine/source/mt/WM_Moving.cpp
		Engine/source/mt/WM_Moving.hpp
		Engine/source/mt/WM_NonClientCalcSize.cpp
		Engine/source/mt/WM_NonClientCalcSize.hpp
		Engine/source/mt/WM_NonClientMouseLeave.cpp
		Engine/source/mt/WM_NonClientMouseLeave.hpp
		Engine/source/mt/WM_NonClientPaint.cpp
		Engine/source/mt/WM_NonClientPaint.hpp
		Engine/source/mt/WM_Paint.cpp
		Engine/source/mt/WM_Paint.hpp
		Engine/source/mt/WM_Quit.cpp
		Engine/source/mt/WM_Quit.hpp
		Engine/source/mt/WM_RightMouseButtonDown.cpp
		Engine/source/mt/WM_RightMouseButtonDown.hpp
		Engine/source/mt/WM_RightMouseButtonUp.cpp
		Engine/source/mt/WM_RightMouseButtonUp.hpp
		Engine/source/mt/WM_SetText.cpp
		Engine/source/mt/WM_SetText.hpp
		Engine/source/mt/WM_Size.cpp
		Engine/source/mt/WM_Size.hpp
		Engine/source/mt/WM_Sizing.cpp
		Engine/source/mt/WM_Sizing.hpp
		Engine/source/mt/WM_Timer.cpp
		Engine/source/mt/WM_Timer.hpp
		Engine/source/mt/WM_WindowPositionChanging.cpp
		Engine/source/mt/WM_WindowPositionChanging.hpp
		EngineDemo/source/EngineDemo.cpp)

