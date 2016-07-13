// Utility.cpp : Utility and Helper functions
//

#include "stdafx.h"
#include "Utility.h"

namespace Utility
{
	void Throw(_In_opt_ LPCWSTR Message)
	{
		OutputDebugString(Message);
		OutputDebugString(L"\n");
		DebugBreak();
	}

	void ThrowOnFail(_In_ HRESULT hr, _In_opt_ LPCWSTR Message)
	{
		if (FAILED(hr))
		{
			Throw(Message);
		}
	}
}