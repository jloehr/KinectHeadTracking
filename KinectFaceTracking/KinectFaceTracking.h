#pragma once

#include "MainWindow.h"
#include "GraphicsContext.h"
#include "Renderer.h"
#include "Model.h"
#include "Transform.h"

class KinectFaceTracking
{
public:
	KinectFaceTracking(_In_ HINSTANCE Instance);

	int Run(_In_ int nCmdShow);

private:
	HINSTANCE Instance;

	MainWindow Window;
	GraphicsContext GraphicsDevice;
	Renderer Renderer;

	Model CubeModel;
	TransformList Cubes;

	void Initialize(_In_ int CmdShow);
	void Destroy();
};
