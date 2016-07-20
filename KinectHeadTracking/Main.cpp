// Main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KinectHeadTracking.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	KinectHeadTracking App(hInstance);

	return App.Run(nCmdShow);
}
