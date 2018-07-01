//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2015 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************


#include <DirectXColors.h>
#include <utility>

#include "Engine.hpp"

using mt::Engine;
using namespace DirectX;

#ifdef _DEBUG
	#include <iostream>
	#include <crtdbg.h>
	#define  _CRTDBG_MAP_ALLOC
	#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	try
	{
		Engine::Initialize(hInstance);
		
		Engine::Run();

 		return 0;
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

class EngineDemo : public Engine
{
public:
	EngineDemo(HINSTANCE hInstance);
	~EngineDemo() = default;

	virtual bool _Initialize(HINSTANCE hInstance) override;

private:
	virtual void OnResize() override;
	virtual void Update() override;
	virtual void Draw() override;

};


bool EngineDemo::_Initialize(HINSTANCE hInstance)
{
	if(!Engine::_Initialize(hInstance))
		return false;
		
	return true;
}

void EngineDemo::OnResize()
{
}

void EngineDemo::Update()
{
}

void EngineDemo::Draw()
{
}
