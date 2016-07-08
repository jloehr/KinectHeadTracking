#pragma once

class MainWindow
{
public:
	MainWindow();

	bool Create(_In_ HINSTANCE Instance);
	void Show(_In_ int CmdShow);

private:
	static const std::wstring WindowClassName;
	static bool WindowClassRegistered;

	static LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	HWND WindowHandle;

	bool RegisterWindowClass(_In_ HINSTANCE Instance);

};

