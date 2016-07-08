#pragma once

#include "resource.h"


class KinectFaceTracking
{
public:
	KinectFaceTracking(_In_ HINSTANCE Instance);

	int Run(_In_ int nCmdShow);

private:
	HINSTANCE Instance;

	void Initialize(_In_ int CmdShow);
	void Destroy();
};
