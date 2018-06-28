#include "InputHandler.hpp"

#include "MathHelper.h"
#include "UploadBuffer.h"
#include "engine.hpp"

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#define  new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace mt;

void InputHandler::on_mouse_down(WPARAM btnState, int x, int y)
{
	_mouse_position.x = x;
	_mouse_position.y = y;

	SetCapture(engine::get_main_window_handle());
}

void InputHandler::on_mouse_up(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void InputHandler::on_mouse_move(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		auto& camera = engine::get_current_camera();

		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - _mouse_position.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - _mouse_position.y));

		camera.Pitch(dx);
		camera.RotateY(dy);

		//// Update angles based on input to orbit camera around box.
		//mTheta += dx;
		//mPhi += dy;

		//// Restrict the angle mPhi.
		//mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		//// Make each pixel correspond to 0.005 unit in the scene.
		//float dx = 0.005f*static_cast<float>(x - _mouse_position.x);
		//float dy = 0.005f*static_cast<float>(y - _mouse_position.y);

		//// Update the camera radius based on input.
		//mRadius += dx - dy;

		//// Restrict the radius.
		//mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}

	_mouse_position.x = x;
	_mouse_position.y = y;
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
void InputHandler::on_key_down(WPARAM vk_key, LPARAM flags)
{

}

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646281(v=vs.85).aspx
void InputHandler::on_key_up(WPARAM vk_key, LPARAM flags)
{
	if (vk_key == VK_ESCAPE)
	{
		PostMessage(engine::get_main_window_handle(), WM_CLOSE, 0, 0);
	}

}
