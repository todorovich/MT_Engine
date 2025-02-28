// Copyright 2018 Micho Todorovich, all rights reserved.

#include "WM_Input.hpp"

#include "Engine.hpp"

using namespace mt::windows;

LRESULT WM_Input::execute(const HWND& hwnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam)
{
	static UINT dwSize = 40;

	// NOT BEING DELETED
	static LPBYTE lpb = new BYTE[dwSize];

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
		sizeof(RAWINPUTHEADER));

	if (lpb == nullptr)
	{
		return 0;
	}

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
		sizeof(RAWINPUTHEADER)) != dwSize)
		OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	static const auto string_buffer_size = 255;

	wchar_t* string_buffer = new wchar_t[string_buffer_size];

	static int result;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		result = swprintf_s(string_buffer, string_buffer_size, L" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n",
			raw->data.keyboard.MakeCode,
			raw->data.keyboard.Flags,
			raw->data.keyboard.Reserved,
			raw->data.keyboard.ExtraInformation,
			raw->data.keyboard.Message,
			raw->data.keyboard.VKey);

		if (FAILED(result))
		{
			// TODO: write error handler
		}

		OutputDebugString(string_buffer);
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		result = swprintf_s(string_buffer, string_buffer_size, L"Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n",
			raw->data.mouse.usFlags,
			raw->data.mouse.ulButtons,
			raw->data.mouse.usButtonFlags,
			raw->data.mouse.usButtonData,
			raw->data.mouse.ulRawButtons,
			raw->data.mouse.lLastX,
			raw->data.mouse.lLastY,
			raw->data.mouse.ulExtraInformation);

		if (FAILED(result))
		{
			// TODO: write error handler
		}
		OutputDebugString(string_buffer);
	}

	delete[] lpb;
	delete[] string_buffer;

	//lpb = nullptr;

	DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));
	DefWindowProc(hwnd, msg, wParam, lParam);
	return 0;
}

WM_Input::WM_Input()
{
	buffer = new char[buffer_size];

	pRawInput = (PRAWINPUT)malloc(sizeof(PRAWINPUT) * 64);
}

WM_Input::~WM_Input()
{
	delete buffer;

	free(pRawInput);
}
