#pragma once

#include "MainWindow.h"
#include "GraphicsContext.h"
#include "Renderer.h"
#include "HeadTracker.h"

#include "DirectionalFoVCamera.h"
#include "FrameCamera.h"

#include "Model.h"
#include "Transform.h"

class KinectHeadTracking
{
public:
	KinectHeadTracking(_In_ HINSTANCE Instance);

	int Run(_In_ int nCmdShow);

private:
	HINSTANCE Instance;

	MainWindow Window;
	GraphicsContext GraphicsDevice;
	Renderer Renderer;
	HeadTracker HeadTracker;

	DirectionalFoVCamera DCamera;
	FrameCamera FCamera;

	Model CubeModel;
	TransformList Cubes;

	void Initialize(_In_ int CmdShow);
	void Release();
};
