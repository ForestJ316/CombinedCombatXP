#include "Hooks.h"
#include "Settings.h"

#include <queue>

static bool canShowXP = true;
static bool playerWasInCombat = false;
static std::queue<bool> NotificationQueue;
static RE::XPChangeData* UpdateTimersXPChangeData;

void Hooks::ResetVars()
{
	canShowXP = true;
	playerWasInCombat = false;
	NotificationQueue = std::queue<bool>();
	UpdateTimersXPChangeData = nullptr;
}

RE::XPChangeData* Hooks::OnXPModifiedSetXPData(RE::XPChangeData* xpData, std::uint32_t playerLevel, float initialXP, float xpAdded, bool syncedXP)
{
	if (!canShowXP || playerWasInCombat || RE::PlayerCharacter::GetSingleton()->IsInCombat())
		return SetXPChangeDataOriginal(xpData, playerLevel, initialXP, xpAdded, false);

	return SetXPChangeDataOriginal(xpData, playerLevel, initialXP, xpAdded, syncedXP);
}

bool Hooks::UpdateCanShowXP(__int64* hudModel, __int64* xpModelData)
{
	static int lastCombatTime = 0;
	int currentTime = RE::GameVM::GetSingleton()->currentTime;
	bool playerInCombat = RE::PlayerCharacter::GetSingleton()->IsInCombat();
	if (playerInCombat)
	{
		lastCombatTime = currentTime;
		// If new combat was started while experience bar was still showing
		if (canShowXP && NotificationQueue.empty())
			ClearSyncedXPUpdate(hudModel);
		canShowXP = false;
		// If Location/Quest Complete notification started before combat then finish it,
		// so that data model doesn't get stuck on false
		if (NotificationQueue.empty())
			return false;
	}
	// Fix for combat with 1 actor
	// (will delay by 1 frame for 2+ actors with this fix)
	else if (!playerInCombat && playerWasInCombat)
	{
		lastCombatTime = currentTime;
		playerWasInCombat = false;
		canShowXP = false;
		if (NotificationQueue.empty())
			return false;
	}
	else if (currentTime - lastCombatTime < Settings::fXPShowDelay * 1000)
	{
		canShowXP = false;
		if (NotificationQueue.empty())
			return false;
	}
	// Case for notification started showing before combat started
	// and new notification after combat ended, with previous one still showing
	if (!canShowXP && NotificationQueue.empty())
		canShowXP = true;
	return CanDisplayXPChange(hudModel, xpModelData);
}

bool Hooks::LocationReadyToShowSyncedXP(__int64* hudModel, RE::XPChangeData* xpData)
{
	if (xpData->syncedXP)
	{
		NotificationQueue.emplace(true);
		return XPReadyToShow(hudModel, xpData);
	}
	else
	{
		return true;
	}
}

void Hooks::LocationSetSyncedXPUpdate(__int64* hudModel, RE::XPChangeData* xpData, __int64 a3)
{
	// Do nothing if xp not synced
	if (xpData->syncedXP)
		SetSyncedXPUpdate(hudModel, xpData, a3);
}

bool Hooks::QuestReadyToShowSyncedXP(__int64* hudModel, RE::XPChangeData* xpData)
{
	if (xpData->syncedXP)
	{
		NotificationQueue.emplace(true);
		return XPReadyToShow(hudModel, xpData);
	}
	else
	{
		return true;
	}
}

void Hooks::QuestSetSyncedXPUpdate(__int64* hudModel, RE::XPChangeData* xpData, __int64 a3)
{
	// Do nothing if xp not synced
	if (xpData->syncedXP)
		SetSyncedXPUpdate(hudModel, xpData, a3);
}

bool Hooks::UpdateDisplayTimersHook(__int64* hudModel, RE::XPChangeData* xpData)
{
	UpdateTimersXPChangeData = xpData;
	// Fix for Location/Quest Completed notifications getting stuck when xp is not showing
	if (!xpData->syncedXP)
		return true;
	// Xp is synced, do regular process
	return XPMeterDoneShowingSyncedXP(hudModel, xpData);
}

void Hooks::TimersClearSyncedXPUpdate(__int64* hudModel)
{
	if (!NotificationQueue.empty())
		NotificationQueue.pop();
	// Return only if xp is synced
	if (UpdateTimersXPChangeData && UpdateTimersXPChangeData->syncedXP)
		return ClearSyncedXPUpdate(hudModel);
}

void Hooks::PlayerUpdateCombat(RE::PlayerCharacter* a_player)
{
	_PlayerUpdateCombat(a_player);

	if (!playerWasInCombat && a_player->IsInCombat())
		playerWasInCombat = true;
}
