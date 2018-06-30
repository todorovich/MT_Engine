
//***************************************************************************************
// Camera.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Simple first person style camera class that lets the viewer explore the 3D scene.
//   -It keeps track of the camera coordinate system relative to the world space
//    so that the view matrix can be constructed.  
//   -It keeps track of the viewing frustum of the camera so that the projection
//    matrix can be obtained.
//***************************************************************************************

#pragma once

#include "precompiled.hpp"

#include <atomic>

#include "d3dUtil.h"

class Camera
{
public:

	Camera();
	Camera(const Camera& other) = delete;
	~Camera();

	// Get/Set world camera position.
	DirectX::XMVECTOR GetPosition()const;
	DirectX::XMFLOAT3 GetPosition3f()const;
	mt::Status SetPosition(float x, float y, float z);
	mt::Status SetPosition(const DirectX::XMFLOAT3& v);
	
	// Get camera basis vectors.
	DirectX::XMVECTOR GetRight()const;
	DirectX::XMFLOAT3 GetRight3f()const;
	DirectX::XMVECTOR GetUp()const;
	DirectX::XMFLOAT3 GetUp3f()const;
	DirectX::XMVECTOR GetLook()const;
	DirectX::XMFLOAT3 GetLook3f()const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;
	
	// Set frustum.
	mt::Status SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	mt::Status LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	mt::Status LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	// Get View/Proj matrices.
	DirectX::XMMATRIX GetView()const;
	DirectX::XMMATRIX GetProj()const;

	DirectX::XMFLOAT4X4 GetView4x4f()const;
	DirectX::XMFLOAT4X4 GetProj4x4f()const;

	// Strafe/Walk the camera a distance d.
	mt::Status Strafe(float d);
	mt::Status Walk(float d);

	// Rotate the camera.
	mt::Status Pitch(float angle);
	mt::Status RotateY(float angle);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

	void SetDirty()
	{
		mViewDirty = true;
	}

private:

// 64 bytes
	DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();

// 12 bytes
	// Camera coordinate system with coordinates relative to world space.
	DirectX::XMFLOAT3 mPosition	= { 0.0f, 0.0f, 10.0f };
	DirectX::XMFLOAT3 mRight	= { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mUp		= { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 mLook		= { 0.0f, 0.0f, 1.0f };

// 4 bytes
	// Cache frustum properties.
	float mNearZ			= 0.0f;
	float mFarZ				= 0.0f;
	float mAspect			= 0.0f;
	float mFovY				= 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight	= 0.0f;

// 1 byte
	bool mViewDirty;
};