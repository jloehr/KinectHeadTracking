#pragma once

class Window
{
public:
	Window();

	void Create(_In_ HINSTANCE Instance);
	void Show(_In_ int CmdShow);

	const HWND & GetHandle() const;

protected:
	HWND WindowHandle;
	virtual const std::wstring & RegisterWindowClass(_In_ HINSTANCE Instance) = 0;
};

