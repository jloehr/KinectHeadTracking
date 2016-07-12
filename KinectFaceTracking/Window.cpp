// Window.cpp : Base class for a window
//

#include "stdafx.h"
#include "Window.h"


Window::Window()
	:WindowHandle(nullptr)
{
}

bool Window::Create(_In_ HINSTANCE Instance)
{
	const std::wstring & WindowClassName = RegisterWindowClass(Instance);
	if (WindowClassName.empty())
	{
		return false;
	}

	WindowHandle = CreateWindow(
		WindowClassName.c_str(),
		L"Kinect Face Tracking",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		nullptr,
		nullptr,
		Instance,
		nullptr
	);

	return WindowHandle != nullptr;
}

void Window::Show(int CmdShow)
{
	ShowWindow(WindowHandle, CmdShow);
}

const HWND & Window::GetHandle() const
{
	return WindowHandle;
}

