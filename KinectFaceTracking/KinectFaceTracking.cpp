// KinectFaceTracking.cpp : Defines the application class.
//

#include "stdafx.h"
#include "KinectFaceTracking.h"

KinectFaceTracking::KinectFaceTracking(_In_ HINSTANCE Instance)
	:Instance(Instance), Window(Renderer), Renderer(GraphicsDevice, Window)
	,Cube(GraphicsDevice)
{
}

int KinectFaceTracking::Run(_In_ int CmdShow)
{
	Initialize(CmdShow);

	// Loop
	MSG Message = {};
	do {
		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		// Update
		// Render
		Renderer.Render(Cube);

	} while (Message.message != WM_QUIT);

	Destroy();

	return static_cast<int>(Message.wParam);
}

void KinectFaceTracking::Initialize(_In_ int CmdShow)
{
	GraphicsDevice.Initialize();

	Window.Create(Instance);
	Renderer.Initialize();

	Window.Show(CmdShow);

	Cube.Create();
}

void KinectFaceTracking::Destroy()
{
	GraphicsDevice.Release();
}
