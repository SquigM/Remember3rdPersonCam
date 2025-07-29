#pragma once

#include <REX/REX.h>

namespace Remember3rdPersonCam
{
	class Config
	{
		REX::INI::Bool<REX::INI::SettingStore> ignore_close;
		REX::INI::Bool<REX::INI::SettingStore> ignore_far;

		Config() :
			ignore_close("Config", "ignore_close", false),
			ignore_far("Config", "ignore_far", false)
		{
			auto store = REX::INI::SettingStore::GetSingleton();
			store->Init(INI_PATH, "");
			store->Load();
			store->Save();
		}

	public:
		static constexpr const char* INI_PATH = "OBSE/Plugins/Remember3rdPersonCam.ini";
		static Config*               GetSingleton()
		{
			static Config config = Config();
			return &config;
		}

		bool IgnoreClose() { return ignore_close.GetValue(); }
		bool IgnoreFar() { return ignore_far.GetValue(); }
	};
}
