#include "Settings.h"

void Settings::Install()
{
	const auto UI = RE::UI::GetSingleton();
	if (UI)
		UI->RegisterSink<RE::MenuModeChangeEvent>(Settings::GetSingleton());
}

Settings::EventResult Settings::ProcessEvent(const RE::MenuModeChangeEvent& a_event, RE::BSTEventSource<RE::MenuModeChangeEvent>*)
{
	if (!a_event.enteringMenuMode && a_event.menuName == "PauseMenu")
		ReadIniSettings();
	return EventResult::kContinue;
}

void Settings::ReadIniSettings()
{
	constexpr auto defaultini_path = L"Data/MCM/Config/CombinedCombatXP/settings.ini";
	constexpr auto userini_path = L"Data/MCM/Settings/CombinedCombatXP.ini";

	const auto ReadSettingsFile = [&](std::filesystem::path path) {
		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(path.string().c_str());

		ReadFloatSetting(ini, "General", "fXPShowDelay", fXPShowDelay);
	};
	// Read Defaults first
	ReadSettingsFile(defaultini_path);
	ReadSettingsFile(userini_path);
}

void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound)
		a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
}
