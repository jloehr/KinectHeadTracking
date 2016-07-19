#pragma once

class Renderer;

class Window
{
public:
	typedef std::pair<ULONG, ULONG> WindowSize;

	Window(_In_ Renderer & WindowRenderer);

	void Create(_In_ HINSTANCE Instance);
	void Show(_In_ int CmdShow);

	const HWND & GetHandle() const;
	const WindowSize & GetWindowSize() const;

protected:
	HWND WindowHandle;
	WindowSize Size;
	Renderer & WindowRenderer;

	WindowSize QueryWindowSize();
	void UpdateWindowSize();

	virtual const std::wstring & RegisterWindowClass(_In_ HINSTANCE Instance) = 0;
};

