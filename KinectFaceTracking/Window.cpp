// Window.cpp : Base class for a window
//

#include "stdafx.h"
#include "Window.h"

#include "Renderer.h"

Window::Window(_In_ Renderer & WindowRenderer)
	:WindowHandle(nullptr), WindowRenderer(WindowRenderer)
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
		this
	);

	if (WindowHandle == nullptr)
	{
		Utility::Throw(L"Failed to create Window!");
	}

	UpdateWindowSize();
}

void Window::Show(int CmdShow)
{
	ShowWindow(WindowHandle, CmdShow);
}

const HWND & Window::GetHandle() const
{
	return WindowHandle;
}

const Window::WindowSize & Window::GetWindowSize() const
{
	return Size;
}

void Window::UpdateWindowSize()
{
	RECT Rect = {};
	GetClientRect(WindowHandle, &Rect);

	Size.first = Rect.right - Rect.left;
	Size.second = Rect.bottom - Rect.top;
}

