#pragma once

// Offsets
typedef RE::XPChangeData* (_fastcall* tSetXPChangeData)(RE::XPChangeData* xpData, std::uint32_t playerLevel, float initialXP, float xpAdded, bool syncedXP);
static REL::Relocation<tSetXPChangeData> SetXPChangeDataOriginal{ REL::ID(115906) };

typedef bool(_fastcall* tCanDisplayXPChange)(__int64* hudModel, __int64* xpModelData);
static REL::Relocation<tCanDisplayXPChange> CanDisplayXPChange{ REL::ID(1541491) };

// Location/Quest Completed synced xp original functions
typedef bool(_fastcall* tXPReadyToShow)(__int64* hudModel, RE::XPChangeData* xpData);
static REL::Relocation<tXPReadyToShow> XPReadyToShow{ REL::ID(170386) };

typedef bool(_fastcall* tSetSyncedXPUpdate)(__int64* hudModel, RE::XPChangeData* xpData, __int64 a3);
static REL::Relocation<tSetSyncedXPUpdate> SetSyncedXPUpdate{ REL::ID(543361) };

// Update Display Timers hook
typedef bool(_fastcall* tXPMeterDoneShowingSyncedXP)(__int64* hudModel, RE::XPChangeData* xpData);
static REL::Relocation<tXPMeterDoneShowingSyncedXP> XPMeterDoneShowingSyncedXP{ REL::ID(1566176) };

typedef void(_fastcall* tClearSyncedXPUpdate)(__int64* hudModel);
static REL::Relocation<tClearSyncedXPUpdate> ClearSyncedXPUpdate{ REL::ID(449586) };

// Hooks
class Hooks
{
public:
	static void Hook()
	{
		// XPChangeData init
		_OnXPModifiedSetXPData = F4SE::GetTrampoline().write_call<5>(REL::ID(569508).address() + 0x49, OnXPModifiedSetXPData);
		// Main function for showing xp
		_UpdateCanShowXP = F4SE::GetTrampoline().write_call<5>(REL::ID(1237281).address() + 0x151, UpdateCanShowXP);

		// Location vignette synced xp check to start displaying location text
		_LocationReadyToShowSyncedXP = F4SE::GetTrampoline().write_call<5>(REL::ID(307644).address() + 0x66, LocationReadyToShowSyncedXP);
		// Location vignette set synced xp update
		_LocationSetSyncedXPUpdate = F4SE::GetTrampoline().write_call<5>(REL::ID(307644).address() + 0x108, LocationSetSyncedXPUpdate);

		// Quest Completed vignette synced xp check to start displaying the text
		_QuestReadyToShowSyncedXP = F4SE::GetTrampoline().write_call<5>(REL::ID(1305825).address() + 0x194, QuestReadyToShowSyncedXP);
		// Quest Completed vignette set synced xp update
		_QuestSetSyncedXPUpdate = F4SE::GetTrampoline().write_call<5>(REL::ID(1305825).address() + 0x230, QuestSetSyncedXPUpdate);;

		// Hook into IsExperienceMeterDoneShowingSyncedXP to start fading the Location/Quest Completed vignette
		_UpdateDisplayTimersHook = F4SE::GetTrampoline().write_call<5>(REL::ID(27517).address() + 0x125, UpdateDisplayTimersHook);
		_TimersClearSyncedXPUpdate = F4SE::GetTrampoline().write_call<5>(REL::ID(27517).address() + 0x18E, TimersClearSyncedXPUpdate);

		// Vfunc hook for combat with 1 actor
		_PlayerUpdateCombat = REL::Relocation<uintptr_t>(RE::VTABLE::PlayerCharacter[0]).write_vfunc(0xFF, PlayerUpdateCombat);

	}
	static void ResetVars();

private:
	static RE::XPChangeData* OnXPModifiedSetXPData(RE::XPChangeData* xpData, std::uint32_t playerLevel, float initialXP, float xpAdded, bool syncedXP);
	static inline REL::Relocation<decltype(OnXPModifiedSetXPData)> _OnXPModifiedSetXPData;

	static bool UpdateCanShowXP(__int64* hudModel, __int64* xpModelData);
	static inline REL::Relocation<decltype(UpdateCanShowXP)> _UpdateCanShowXP;

	// Location vignette
	static bool LocationReadyToShowSyncedXP(__int64* hudModel, RE::XPChangeData* xpData);
	static inline REL::Relocation<decltype(LocationReadyToShowSyncedXP)> _LocationReadyToShowSyncedXP;

	static void LocationSetSyncedXPUpdate(__int64* hudModel, RE::XPChangeData* xpData, __int64 a3);
	static inline REL::Relocation<decltype(LocationSetSyncedXPUpdate)> _LocationSetSyncedXPUpdate;

	// Quest Completed vignette
	static bool QuestReadyToShowSyncedXP(__int64* hudModel, RE::XPChangeData* xpData);
	static inline REL::Relocation<decltype(QuestReadyToShowSyncedXP)> _QuestReadyToShowSyncedXP;

	static void QuestSetSyncedXPUpdate(__int64* hudModel, RE::XPChangeData* xpData, __int64 a3);
	static inline REL::Relocation<decltype(QuestSetSyncedXPUpdate)> _QuestSetSyncedXPUpdate;

	// Update Display Timers hook
	static bool UpdateDisplayTimersHook(__int64* hudModel, RE::XPChangeData* xpData);
	static inline REL::Relocation<decltype(UpdateDisplayTimersHook)> _UpdateDisplayTimersHook;

	static void TimersClearSyncedXPUpdate(__int64* hudModel);
	static inline REL::Relocation<decltype(TimersClearSyncedXPUpdate)> _TimersClearSyncedXPUpdate;

	// Vfunc
	static void PlayerUpdateCombat(RE::PlayerCharacter* a_player);
	static inline REL::Relocation<decltype(PlayerUpdateCombat)> _PlayerUpdateCombat;
};
