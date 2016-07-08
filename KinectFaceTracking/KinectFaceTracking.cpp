// KinectFaceTracking.cpp : Defines the application class.
//

#include "stdafx.h"
#include "KinectFaceTracking.h"

KinectFaceTracking::KinectFaceTracking(_In_ HINSTANCE Instance)
	:Instance(Instance)
{
}

int KinectFaceTracking::Run(_In_ int nCmdShow)
{
	Initialize(nCmdShow);

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

	} while (Message.message != WM_QUIT);

	Destroy();

	return static_cast<int>(Message.wParam);
}

void KinectFaceTracking::Initialize(_In_ int nCmdShow)
{
}

void KinectFaceTracking::Destroy()
{
}
