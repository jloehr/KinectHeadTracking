// KinectFaceTracking.cpp : Defines the application class.
//

#include "stdafx.h"
#include "KinectFaceTracking.h"

KinectFaceTracking::KinectFaceTracking(_In_ HINSTANCE Instance)
	:Instance(Instance), Window(Renderer), Renderer(GraphicsDevice, Window)
	,CubeModel(GraphicsDevice)
{
	Cubes = { 
		Transform(Vector3(-5.0f,-5.0f, -0.5f), Quaternion(),                   Vector3(0.5f)),
		Transform(Vector3(-5.0f, 5.0f, -0.5f), Quaternion( 90.f,   0.f,  0.f), Vector3(0.5f)),
		Transform(Vector3( 5.0f,-5.0f, -0.5f), Quaternion( 90.f,  90.f,  0.f), Vector3(0.5f)),
		Transform(Vector3( 5.0f, 5.0f, -0.5f), Quaternion(180.f,  90.f,  0.f), Vector3(0.5f)),
		Transform(Vector3(-5.0f,-5.0f,-10.0f), Quaternion(  0.f,   0.f,  0.f), Vector3(0.5f)),
		Transform(Vector3(-5.0f, 5.0f,-10.0f), Quaternion( 90.f,   0.f,  0.f), Vector3(0.5f)),
		Transform(Vector3( 5.0f,-5.0f,-10.0f), Quaternion(  0.f,  90.f,  0.f), Vector3(0.5f)),
		Transform(Vector3( 5.0f, 5.0f,-10.0f), Quaternion(180.f,  90.f,  0.f), Vector3(0.5f)),
		Transform(Vector3( 1.0f, 0.0f, -2.0f), Quaternion(  0.f,   0.f,  0.f)),
		Transform(Vector3(-1.0f, 0.0f, -2.0f), Quaternion(  0.f, -45.f,  0.f), Vector3(1.5f))
	};
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

		Renderer.Render({ Renderer::RenderParameter(CubeModel, Cubes) });

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

	CubeModel.Create();
}

void KinectFaceTracking::Destroy()
{
	GraphicsDevice.Release();
}
