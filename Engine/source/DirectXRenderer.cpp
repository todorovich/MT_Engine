#pragma once

#include "precompiled.hpp"

#include "DirectXRenderer.hpp"

#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

void DirectXRenderer::render()
{
	_is_rendering = true;

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(dx_command_list->Reset(dx_command_list_allocator.Get(), mPSO.Get()));

	dx_command_list->RSSetViewports(1, &mScreenViewport);
	dx_command_list->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	dx_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(get_current_back_buffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Clear the back buffer and depth buffer.
	dx_command_list->ClearRenderTargetView(get_current_back_buffer_view(), Colors::LightSteelBlue, 0, nullptr);
	dx_command_list->ClearDepthStencilView(get_depth_stencil_view(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	dx_command_list->OMSetRenderTargets(1, &get_current_back_buffer_view(), true, &get_depth_stencil_view());

	ID3D12DescriptorHeap* descriptorHeaps[] = { dx_cbv_heap.Get() };
	dx_command_list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	dx_command_list->SetGraphicsRootSignature(dx_root_signature.Get());

	dx_command_list->IASetVertexBuffers(0, 1, &box_mesh_geometry->VertexBufferView());
	dx_command_list->IASetIndexBuffer(&box_mesh_geometry->IndexBufferView());
	dx_command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dx_command_list->SetGraphicsRootDescriptorTable(0, dx_cbv_heap->GetGPUDescriptorHandleForHeapStart());

	dx_command_list->DrawIndexedInstanced(
		box_mesh_geometry->DrawArgs["box"].IndexCount,
		1, 0, 0, 0);

	// Indicate a state transition on the resource usage.
	dx_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(get_current_back_buffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(dx_command_list->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { dx_command_list.Get() };
	dx_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(dx_swap_chain->Present(0, 0));
	current_back_buffer = (current_back_buffer + 1) % _swap_chain_buffer_count;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	//flush_command_queue();
	
	// NOTE: Moved this to happen outside of the render as it ends up syncing the render time
	// to the framerate. Instead I moved it to flush just before the idle is reset, this should
	// force the game to actually stay synced with the gpu without screwing up the stats. Given
	// the note above I imagine a real fix is incoming.

	_is_rendering = false;
}

bool DirectXRenderer::initialize_direct3d(HWND main_window_handle)
{
	_main_window_handle = main_window_handle;

#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	ComPtr<ID3D12Debug> debugController;

	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));

	debugController->EnableDebugLayer();
#endif

	// Create DirectX Graphics Infrastructure 1.1 factory that you can use to generate other DXGI objects
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dx_dxgi_factory)));

	// Try to create hardware device.
	// param 1 is the adapter to use, nullptr for default
	HRESULT hardwareResult = D3D12CreateDevice(nullptr,	D3D_FEATURE_LEVEL_11_0,	IID_PPV_ARGS(&dx_device));

	// If we failed to create the D3D12 Device then try to fall back to Windows Advanced Rasterization Platform (WARP)(software) device
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
	
		ThrowIfFailed(dx_dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(pWarpAdapter.Get(),	D3D_FEATURE_LEVEL_11_0,	IID_PPV_ARGS(&dx_device))); 
	}

	// create a fence
	ThrowIfFailed(dx_device->CreateFence(0, D3D12_FENCE_FLAG_NONE,	IID_PPV_ARGS(&fence)));

	rtv_descriptor_size		 = dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dsv_descriptor_size		 = dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	cbv_srv_uav_descriptor_size = dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

	msQualityLevels.Format			 = mBackBufferFormat;
	msQualityLevels.SampleCount		 = 4;
	msQualityLevels.Flags			 = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	
	ThrowIfFailed(dx_device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	_4x_msaa_quality = msQualityLevels.NumQualityLevels;
	
	assert(_4x_msaa_quality > 0 && "Unexpected MSAA quality level.");

#ifdef _DEBUG
	// This is just annoying, but could be useful in the future when logs are better.
	//log_adapters();
#endif

	create_dx_command_objects();
	
	create_swap_chain();
	
	create_descriptor_heaps();

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(dx_command_list->Reset(dx_command_list_allocator.Get(), nullptr));

	create_constant_buffers();
	
	create_root_signature();
	
	create_shaders_and_input_layout();
	
	create_box_geometry();
	
	create_pipeline_state_object();

	// Execute the initialization commands.
	ThrowIfFailed(dx_command_list->Close());

	ID3D12CommandList* cmdsLists[] = { dx_command_list.Get() };
	dx_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	flush_command_queue();

	_is_initialized = true;

	return true;
}

void DirectXRenderer::create_dx_command_objects()
{
	D3D12_COMMAND_QUEUE_DESC command_queue_description = {};
	command_queue_description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	command_queue_description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// Create a command queue
	ThrowIfFailed(dx_device->CreateCommandQueue(&command_queue_description, IID_PPV_ARGS(&dx_command_queue)));

	// Create a command allocator
	ThrowIfFailed(dx_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(dx_command_list_allocator.GetAddressOf())));

	// Create a command list
	ThrowIfFailed(dx_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		dx_command_list_allocator.Get(),				// Associated command allocator
		nullptr,										// Initial PipelineStateObject
		IID_PPV_ARGS(dx_command_list.GetAddressOf())));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	dx_command_list->Close();
}

void DirectXRenderer::create_swap_chain()
{
	// Releases all references for the pointer to the interface that is associated with this ComPtr.
	// Release the previous swapchain we will be recreating.
	dx_swap_chain.Reset();

	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	swap_chain_description.BufferDesc.Width						= _client_width;
	swap_chain_description.BufferDesc.Height					= _client_height;
	swap_chain_description.BufferDesc.RefreshRate.Numerator		= 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator	= 1;
	swap_chain_description.BufferDesc.Format					= mBackBufferFormat;
	swap_chain_description.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_description.SampleDesc.Count						= _4x_msaa_state ? 4 : 1;
	swap_chain_description.SampleDesc.Quality					= _4x_msaa_state ? (_4x_msaa_quality - 1) : 0;
	swap_chain_description.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.BufferCount							= _swap_chain_buffer_count;
	swap_chain_description.OutputWindow							= _main_window_handle;
	swap_chain_description.Windowed								= true;
	swap_chain_description.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_description.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ThrowIfFailed(dx_dxgi_factory->CreateSwapChain(
		dx_command_queue.Get(),
		&swap_chain_description,
		dx_swap_chain.GetAddressOf()));
}

void DirectXRenderer::create_descriptor_heaps()
{
	// Create the Render-Target-View (RTV) Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_description;
	rtv_heap_description.NumDescriptors = get_swap_chain_buffer_count();
	rtv_heap_description.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_heap_description.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtv_heap_description.NodeMask		= 0;

	// Create the Render-Target-View (RTV) Descriptor-Heap from the provided description
	ThrowIfFailed(dx_device->CreateDescriptorHeap(
		&rtv_heap_description, IID_PPV_ARGS(dx_rtv_heap.GetAddressOf())));

	// Create the Depth-Stencil-View (DSV) Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_description;
	dsv_heap_description.NumDescriptors		= 1;
	dsv_heap_description.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsv_heap_description.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsv_heap_description.NodeMask			= 0;

	// Create the Depth-Stencil-View (DSV) Descriptor-Heap from the provided description
	ThrowIfFailed(dx_device->CreateDescriptorHeap(
		&dsv_heap_description, IID_PPV_ARGS(dx_dsv_heap.GetAddressOf())));

	// Create Constant-Buffer-View/Shader-Resource-View/Unordered-Access-View Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC cbv_heap_description;
	cbv_heap_description.NumDescriptors	= 1;
	cbv_heap_description.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbv_heap_description.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbv_heap_description.NodeMask		= 0;

	// Create other CBV/SRV/UAV Descriptor-Heap from the descriptor
	ThrowIfFailed(dx_device->CreateDescriptorHeap(&cbv_heap_description,
		IID_PPV_ARGS(&dx_cbv_heap)));

}

void DirectXRenderer::create_constant_buffers()
{
	object_constants_upload_buffer = std::make_unique<UploadBuffer<ObjectConstants>>(dx_device.Get(), 1, true);

	UINT object_constant_buffer_size_bytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	D3D12_GPU_VIRTUAL_ADDRESS constant_buffer_address = object_constants_upload_buffer->Resource()->GetGPUVirtualAddress();
	// Offset to the ith object constant buffer in the buffer.
	// int boxCBufIndex = 0;
	// cbAddress += boxCBufIndex*object_constant_buffer_size_bytes;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_description;
	cbv_description.BufferLocation	= constant_buffer_address;
	cbv_description.SizeInBytes		= d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	dx_device->CreateConstantBufferView(
		&cbv_description,
		dx_cbv_heap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXRenderer::create_root_signature()
{
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig	= nullptr;
	ComPtr<ID3DBlob> errorBlob			= nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	ThrowIfFailed(hr);

	ThrowIfFailed(dx_device->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&dx_root_signature)));
}

void DirectXRenderer::create_shaders_and_input_layout()
{
	HRESULT hr = S_OK;

	mvsByteCode = d3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
	mpsByteCode = d3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

	// D3D_INPUT_ELEMENT_DESC
	mInputLayout =
	{
		// Semantic Name,									Input Slot,											Instanced Data Step Rate						 
		//              Semantic Index,							Aligned Byte Offset,
		//					Format,									Input Slot Class, 
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0 },
		{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0 }
	};
}

void DirectXRenderer::create_box_geometry()
{
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	box_mesh_geometry = std::make_unique<MeshGeometry>();
	box_mesh_geometry->Name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &box_mesh_geometry->VertexBufferCPU));
	CopyMemory(box_mesh_geometry->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &box_mesh_geometry->IndexBufferCPU));
	CopyMemory(box_mesh_geometry->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	box_mesh_geometry->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(dx_device.Get(),
		dx_command_list.Get(), vertices.data(), vbByteSize, box_mesh_geometry->VertexBufferUploader);

	box_mesh_geometry->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(dx_device.Get(),
		dx_command_list.Get(), indices.data(), ibByteSize, box_mesh_geometry->IndexBufferUploader);

	box_mesh_geometry->VertexByteStride = sizeof(Vertex);
	box_mesh_geometry->VertexBufferByteSize = vbByteSize;
	box_mesh_geometry->IndexFormat = DXGI_FORMAT_R16_UINT;
	box_mesh_geometry->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	box_mesh_geometry->DrawArgs["box"] = submesh;
}

void DirectXRenderer::create_pipeline_state_object()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = dx_root_signature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = get_4x_msaa_state() ? 4 : 1;
	psoDesc.SampleDesc.Quality = get_4x_msaa_state() ? (_4x_msaa_quality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(dx_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}

void DirectXRenderer::flush_command_queue()
{
	// Advance the fence value to mark commands up to this fence point.
	current_fence_index++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(dx_command_queue->Signal(fence.Get(), current_fence_index));

	// Wait until the GPU has completed commands up to this fence point.
	if (fence->GetCompletedValue() < current_fence_index)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(fence->SetEventOnCompletion(current_fence_index, eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);

		CloseHandle(eventHandle);
	}
}

ID3D12Resource* DirectXRenderer::get_current_back_buffer() const
{
	return swap_chain_buffer[current_back_buffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXRenderer::get_current_back_buffer_view() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		dx_rtv_heap->GetCPUDescriptorHandleForHeapStart(),
		current_back_buffer,
		rtv_descriptor_size);
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXRenderer::get_depth_stencil_view() const
{
	return dx_dsv_heap->GetCPUDescriptorHandleForHeapStart();
}

void DirectXRenderer::log_adapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (dx_dxgi_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		log_adapter_outputs(adapterList[i]);
		ReleaseCom(adapterList[i]);
	}
}

void DirectXRenderer::log_adapter_outputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		log_output_display_modes(output, mBackBufferFormat);

		ReleaseCom(output);

		++i;
	}
}

void DirectXRenderer::log_output_display_modes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}

void DirectXRenderer::resize(int client_width, int client_height)
{
	if (client_width != _client_width || client_height != _client_height)
	{
		_client_width = client_width;
		_client_height = client_height;

		assert(dx_device);
		assert(dx_swap_chain);
		assert(dx_command_list_allocator);

		// Flush before changing any resources.
		flush_command_queue();

		ThrowIfFailed(dx_command_list->Reset(dx_command_list_allocator.Get(), nullptr));

		// Release the previous resources we will be recreating.
		for (int i = 0; i < _swap_chain_buffer_count; ++i)
			swap_chain_buffer[i].Reset();
		depth_stencil_buffer.Reset();

		// Resize the swap chain.
		ThrowIfFailed(dx_swap_chain->ResizeBuffers(
			_swap_chain_buffer_count,
			_client_width, _client_height,
			mBackBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		current_back_buffer = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(dx_rtv_heap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < _swap_chain_buffer_count; i++)
		{
			ThrowIfFailed(dx_swap_chain->GetBuffer(i, IID_PPV_ARGS(&swap_chain_buffer[i])));
			dx_device->CreateRenderTargetView(swap_chain_buffer[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, rtv_descriptor_size);
		}

		// Create the depth/stencil buffer and view.
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = _client_width;
		depthStencilDesc.Height = _client_height;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.Format = mDepthStencilFormat;
		depthStencilDesc.SampleDesc.Count = _4x_msaa_state ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = _4x_msaa_state ? (_4x_msaa_quality - 1) : 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = mDepthStencilFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;
		ThrowIfFailed(dx_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(depth_stencil_buffer.GetAddressOf())));

		// Create descriptor to mip level 0 of entire resource using the format of the resource.
		dx_device->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, get_depth_stencil_view());

		// Transition the resource from its initial state to be used as a depth buffer.
		dx_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(depth_stencil_buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		// Execute the resize commands.
		ThrowIfFailed(dx_command_list->Close());
		ID3D12CommandList* cmdsLists[] = { dx_command_list.Get() };
		dx_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until resize is complete.
		flush_command_queue();

		// Update the viewport transform to cover the client area.
		mScreenViewport.TopLeftX = 0;
		mScreenViewport.TopLeftY = 0;
		mScreenViewport.Width = static_cast<float>(_client_width);
		mScreenViewport.Height = static_cast<float>(_client_height);
		mScreenViewport.MinDepth = 0.0f;
		mScreenViewport.MaxDepth = 1.0f;

		mScissorRect = { 0, 0, _client_width, _client_height };

		_client_aspect_ratio = static_cast<float>(_client_width) / _client_height;

		// The window resized, so update the aspect ratio and recompute the projection matrix.
		engine::get_current_camera().SetLens(0.25f * MathHelper::Pi, engine::get_aspect_ratio(), 1.0f, 1000.0f);
		//XMStoreFloat4x4(&mProj, P);
	}
}

void DirectXRenderer::Set4xMsaaState(bool value)
{
	if (_4x_msaa_state != value)
	{
		_4x_msaa_state = value;

		// Recreate the swapchain and buffers with new multisample settings.
		create_swap_chain();
		resize(_client_width, _client_height);
	}
}

void DirectXRenderer::Update()
{
	camera.Lock();

	camera.UpdateViewMatrix();

	XMMATRIX worldViewProj = camera.GetView() * camera.GetProj();

	camera.Unlock();

	// Update the constant buffer with the latest worldViewProj matrix.
	ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	object_constants_upload_buffer->CopyData(0, objConstants);

}