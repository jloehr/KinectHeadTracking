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
		L"Kinect Head Tracking",
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

	Size = QueryWindowSize();
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

Window::WindowSize Window::QueryWindowSize()
{
	RECT Rect = {};
	GetClientRect(WindowHandle, &Rect);

	return WindowSize(Rect.right - Rect.left, Rect.bottom - Rect.top);
}

void Window::UpdateWindowSize()
{
	WindowSize NewSize = QueryWindowSize();

	if (NewSize != Size)
	{
		Size = NewSize;
		WindowRenderer.OnWindowSizeChange(Size);
	}
}

