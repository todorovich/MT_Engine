#pragma once

//#include "../mt/TimerManager.hpp"
#include "d3dUtil.h"
#include "UploadBuffer.h"
#include "Camera.hpp"

#include <string>

// Link necessary d3d12 libraries.
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

class DirectXRenderer
{
public:
	DirectXRenderer()
	{
		//auto string = L"sizeof vector: " + std::to_wstring(sizeof(camera)) + L" bytes \n";

		//OutputDebugStringW(string.c_str());
	}

	DirectXRenderer(const DirectXRenderer&) = default;

	~DirectXRenderer()
	{
		if (dx_device != nullptr)
			flush_command_queue();
	}

// Accessors
	Camera& GetCurrentCamera() { return camera; } // NOT CONST!!!!
	
	bool get_4x_msaa_state() const { return _4x_msaa_state; };

	bool get_is_initialized() const { return _is_initialized; };

	int  get_swap_chain_buffer_count() const { return _swap_chain_buffer_count; };

	void Set4xMsaaState(bool value);

	bool get_is_rendering() const { return _is_rendering; }

// Mutators
	bool initialize_direct3d(HWND main_window_handle);

	void render();

	void Resize(int client_width, int client_height);

	void Update();

	void flush_command_queue();

protected:

// Accessors
	ID3D12Resource* get_current_back_buffer()const;

	D3D12_CPU_DESCRIPTOR_HANDLE get_current_back_buffer_view()const;

	D3D12_CPU_DESCRIPTOR_HANDLE get_depth_stencil_view()const;

// Mutators

	void create_command_list();

	void create_dx_command_objects();

	void create_swap_chain();

	virtual void create_descriptor_heaps();
	
	void create_constant_buffers();

	void create_root_signature();

	void create_shaders_and_input_layout();

	void create_box_geometry();

	void create_pipeline_state_object();

	void log_adapters();

	void log_adapter_outputs(IDXGIAdapter* adapter);
	
	void log_output_display_modes(IDXGIOutput* output, DXGI_FORMAT format);

// Data
	static const int							_swap_chain_buffer_count = 2;

	Camera										camera; // 204 bytes (getting kind of bloated)

// 64 byte types
	XMFLOAT4X4									mWorld = MathHelper::Identity4x4(); // Transformation from Local Space to World Space 
	XMFLOAT4X4									mView  = MathHelper::Identity4x4();	// Transformation from World Space to the camera's View Space
	XMFLOAT4X4									mProj  = MathHelper::Identity4x4();	// Transformation from View Space to Projection Space (Normalized Device Coordinates (GPU Space?))

// 32 byte type
	vector<D3D12_INPUT_ELEMENT_DESC>			mInputLayout; // 32 bytes according to sizeof

// 24 byte type
	D3D12_VIEWPORT								mScreenViewport;

// 16 byte types
	D3D12_RECT									mScissorRect;

// Pointers - 8 byte each
	HWND										_main_window_handle;

	unique_ptr<UploadBuffer<ObjectConstants>>	object_constants_upload_buffer = nullptr;
	unique_ptr<MeshGeometry>					box_mesh_geometry			   = nullptr;

	ComPtr<IDXGIFactory4>						dx_dxgi_factory;
	ComPtr<IDXGISwapChain>						dx_swap_chain;
	ComPtr<ID3D12Device>						dx_device;

	ComPtr<ID3D12Fence>							fence;

	ComPtr<ID3D12CommandQueue>					dx_command_queue;
	ComPtr<ID3D12CommandAllocator>				dx_command_list_allocator;
	ComPtr<ID3D12GraphicsCommandList>			dx_command_list;

	ComPtr<ID3D12Resource>						swap_chain_buffer[_swap_chain_buffer_count];
	ComPtr<ID3D12Resource>						depth_stencil_buffer;

	ComPtr<ID3D12DescriptorHeap>				dx_rtv_heap = nullptr;
	ComPtr<ID3D12DescriptorHeap>				dx_dsv_heap = nullptr;

	ComPtr<ID3D12RootSignature>					dx_root_signature	= nullptr;
	ComPtr<ID3D12DescriptorHeap>				dx_cbv_heap			= nullptr;

	ComPtr<ID3DBlob>							mvsByteCode = nullptr;
	ComPtr<ID3DBlob>							mpsByteCode = nullptr;
	ComPtr<ID3D12PipelineState>					mPSO		= nullptr;

// 8 byte types
	UINT64										current_fence_index = 0;

	POINT										mLastMousePos;

// 4 byte types
	UINT										rtv_descriptor_size			= 0;
	UINT										dsv_descriptor_size			= 0;
	UINT										cbv_srv_uav_descriptor_size = 0;
	UINT										_4x_msaa_quality			= 0;      // quality level of 4X MSAA

	D3D_DRIVER_TYPE								md3dDriverType		= D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT									mBackBufferFormat	= DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT									mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	int		current_back_buffer		= 0; 
	int		_window_width			= 0;
	int		_window_height			= 0;

	float	_window_aspect_ratio	= 0.0f;

// Single byte each
	bool	_4x_msaa_state			= false;
	bool	_is_initialized			= false;
	bool    _is_rendering			= false;
};