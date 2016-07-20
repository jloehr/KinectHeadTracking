// Utility.cpp : Utility and Helper functions
//

#include "stdafx.h"
#include "Utility.h"

namespace Utility
{
	void Throw(_In_opt_ LPCWSTR Message)
	{
		if (Message != nullptr)
		{
			OutputDebugStringW(Message);
			OutputDebugStringW(L"\n");
		}

		DebugBreak();
	}

	void ThrowOnFail(_In_ HRESULT hr, _In_opt_ LPCWSTR Message)
	{
		if (FAILED(hr))
		{
			Throw(Message);
		}
	}

	void ThrowOnFail(HRESULT hr, Microsoft::WRL::ComPtr<ID3DBlob> & Error)
	{
		if (FAILED(hr))
		{
			std::wstring Message;

			if (Error)
			{
				std::string ANSIMessage(static_cast<const char *>(Error->GetBufferPointer()));
				Message.assign(ANSIMessage.begin(), ANSIMessage.end());
			}

			Throw(Message.c_str());
		}
	}
}
