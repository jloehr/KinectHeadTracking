// SettingsFile.cpp : Loads settings from a "Settings.ini" file
//

#include "stdafx.h"
#include "SettingsFile.h"

namespace SettingsFile
{
	static const std::wstring Filename = L"Settings.ini";

	static const std::wstring & GetSettingsFilePath();
	static void LoadString(const std::wstring & Section, const std::wstring & Key, std::wstring & Value);
	static float LoadFloat(const std::wstring & Section, const std::wstring & Key, float DefaultValue);

	namespace Monitor 
	{
		static const std::wstring SectionName = L"Monitor";

		namespace MonitorHeight
		{
			static const std::wstring Key = L"Monitorheight";
			static const float Default = 15.f;
		}

		float GetMonitorHeight()
		{
			return LoadFloat(SectionName, MonitorHeight::Key, MonitorHeight::Default);
		}
	};

	namespace Kinect 
	{
		static const std::wstring SectionName = L"Kinect";

		namespace KinectOffset
		{
			static const std::wstring KeyX = L"OffsetX";
			static const std::wstring KeyY = L"OffsetY";
			static const std::wstring KeyZ = L"OffsetZ";
			static const float Default = 0.f;
		}

		Vector3 GetKinectOffset()
		{
			float X = LoadFloat(SectionName, KinectOffset::KeyX, KinectOffset::Default);
			float Y = LoadFloat(SectionName, KinectOffset::KeyY, KinectOffset::Default);
			float Z = LoadFloat(SectionName, KinectOffset::KeyZ, KinectOffset::Default);
			
			return Vector3(X, Y, Z);
		}
	};

	static const std::wstring & GetSettingsFilePath()
	{
		static std::wstring Path;

		if (Path.empty())
		{
			std::array<wchar_t, 1024> Buffer;
			DWORD CharactersCopied = GetModuleFileName(nullptr, Buffer.data(), static_cast<DWORD>(Buffer.size()));

			if (CharactersCopied == 0)
			{
				return Path;
			}

			Path.assign(Buffer.data(), CharactersCopied);

			size_t LastSlashPosition = Path.rfind(L'\\');
			Path.replace(LastSlashPosition + 1, std::wstring::npos, Filename);
		}

		return Path;
	}

	static void LoadString(const std::wstring & Section, const std::wstring & Key, std::wstring & Value)
	{
		std::array<wchar_t, 256> Buffer;

		DWORD CharactersCopied = GetPrivateProfileString(Section.c_str(), Key.c_str(), nullptr, Buffer.data(), static_cast<DWORD>(Buffer.size()), GetSettingsFilePath().c_str());

		Value.assign(Buffer.data(), CharactersCopied);
	}

	static float LoadFloat(const std::wstring & Section, const std::wstring & Key, float DefaultValue)
	{
		std::wstring ValueAsString;

		LoadString(Section, Key, ValueAsString);

		try
		{
			return stof(ValueAsString, nullptr);
		}
		catch (const std::invalid_argument & e)
		{
			UNREFERENCED_PARAMETER(e);
			return DefaultValue;
		}
	}
};