// MainWindow.cpp : Class for the main window
//

#include "stdafx.h"
#include "MainWindow.h"

const std::wstring MainWindow::WindowClassName = L"Main Windows";
bool MainWindow::WindowClassRegistered = false;

MainWindow::MainWindow()
	:WindowHandle(nullptr)
{
}

bool MainWindow::Create(_In_ HINSTANCE Instance)
{
	if (!RegisterWindowClass(Instance))
	{
		return false;
	}

	WindowHandle = CreateWindow(
		MainWindow::WindowClassName.c_str(),
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

void MainWindow::Show(int CmdShow)
{
	ShowWindow(WindowHandle, CmdShow);
}

bool MainWindow::RegisterWindowClass(_In_ HINSTANCE Instance)
{
	if (WindowClassRegistered)
	{
		return true;
	}

	WNDCLASSEXW WindowClass = { 0 };

	WindowClass.cbSize = sizeof(WNDCLASSEX);

	WindowClass.lpfnWndProc = MainWindow::WindowProc;
	WindowClass.hInstance = Instance;
	WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WindowClass.lpszClassName = MainWindow::WindowClassName.c_str();
	
	WindowClassRegistered = (RegisterClassEx(&WindowClass) != 0);

	return WindowClassRegistered;
}

LRESULT MainWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
