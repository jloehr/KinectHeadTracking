#pragma once

#include "Window.h"

class MainWindow : public Window
{
public:
	MainWindow(_In_ Renderer & WindowRenderer);

private:
	static const std::wstring WindowClassName;
	static bool WindowClassRegistered;
	static LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	virtual const std::wstring & RegisterWindowClass(_In_ HINSTANCE Instance);

};
