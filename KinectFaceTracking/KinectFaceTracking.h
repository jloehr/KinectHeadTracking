#pragma once

#include "MainWindow.h"

class KinectFaceTracking
{
public:
	KinectFaceTracking(_In_ HINSTANCE Instance);

	int Run(_In_ int nCmdShow);

private:
	HINSTANCE Instance;

	MainWindow Window;

	void Initialize(_In_ int CmdShow);
	void Destroy();
};
