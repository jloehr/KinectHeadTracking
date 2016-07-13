// MainWindow.cpp : Class for the main window
//

#include "stdafx.h"
#include "MainWindow.h"

const std::wstring MainWindow::WindowClassName = L"Main Windows";
bool MainWindow::WindowClassRegistered = false;

MainWindow::MainWindow()
{
}

const std::wstring & MainWindow::RegisterWindowClass(_In_ HINSTANCE Instance)
{
	if (!WindowClassRegistered)
	{
		WNDCLASSEXW WindowClass = { 0 };

		WindowClass.cbSize = sizeof(WNDCLASSEX);

		WindowClass.lpfnWndProc = MainWindow::WindowProc;
		WindowClass.hInstance = Instance;
		WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WindowClass.lpszClassName = MainWindow::WindowClassName.c_str();

		WindowClassRegistered = (RegisterClassEx(&WindowClass) != 0);

		if (!WindowClassRegistered)
		{
			Utility::Throw(L"Failed to Register Main Window Class");
		}
	}

	return MainWindow::WindowClassName;
}

LRESULT MainWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MainWindow * Window = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (uMsg)
	{
		case WM_CREATE:
		{
			LPCREATESTRUCT CreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(CreateStruct->lpCreateParams));
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
