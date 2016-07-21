// KinectHeadTracking.cpp : Defines the application class.
//

#include "stdafx.h"
#include "KinectHeadTracking.h"

KinectHeadTracking::KinectHeadTracking(_In_ HINSTANCE Instance)
	:Instance(Instance), Window(Renderer), Renderer(GraphicsDevice, Window, FCamera), HeadTracker(FCamera, Vector3(0.f, -0.2f, 0.f), 10.f / 0.3f)
	,CubeModel(GraphicsDevice)
	,DCamera(Vector3(0.0f, 0.0f, 5.0f))
	,FCamera(Vector3(0.0f, 0.0f, 5.0f), 10.0f)
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
		Transform(Vector3( 1.5f, 0.0f, -5.0f), Quaternion(  0.f,   0.f,  0.f), Vector3(1.0f)),
		Transform(Vector3(-1.5f, 0.0f, -5.0f), Quaternion(  0.f, -45.f,  0.f), Vector3(2.0f))
	};
}

int KinectHeadTracking::Run(_In_ int CmdShow)
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

		HeadTracker.Update();
		Renderer.Render({ Renderer::RenderParameter(CubeModel, Cubes) });

	} while (Message.message != WM_QUIT);

	Release();

	return static_cast<int>(Message.wParam);
}

void KinectHeadTracking::Initialize(_In_ int CmdShow)
{
	Window.Create(Instance);

	GraphicsDevice.Initialize();
	Renderer.Initialize();
	CubeModel.Create();

	HeadTracker.Initialize();

	Window.Show(CmdShow);
}

void KinectHeadTracking::Release()
{
	GraphicsDevice.Release(); 
	HeadTracker.Release();
}
