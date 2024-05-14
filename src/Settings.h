#pragma once

class Settings :
	public RE::BSTEventSink<RE::MenuModeChangeEvent>
{
private:
	using EventResult = RE::BSEventNotifyControl;

public:
	static Settings* GetSingleton()
	{
		static Settings singleton;
		return std::addressof(singleton);
	}
	static void Install();
	virtual EventResult ProcessEvent(const RE::MenuModeChangeEvent& a_event, RE::BSTEventSource<RE::MenuModeChangeEvent>* a_eventSource) override;

	static void ReadIniSettings();
	static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);

	// General
	static inline float fXPShowDelay = 3.0f;
};
