// Window.cpp : Base class for a window
//

#include "stdafx.h"
#include "Window.h"


Window::Window()
	:WindowHandle(nullptr)
{
}

void Window::Create(_In_ HINSTANCE Instance)
{
	const std::wstring & WindowClassName = RegisterWindowClass(Instance);
	if (WindowClassName.empty())
	{
		Utility::Throw(L"Window Class Name is empty!");
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

	if (WindowHandle == nullptr)
	{
		Utility::Throw(L"Failed to create Window!");
	}
}

void Window::Show(int CmdShow)
{
	ShowWindow(WindowHandle, CmdShow);
}

const HWND & Window::GetHandle() const
{
	return WindowHandle;
}

