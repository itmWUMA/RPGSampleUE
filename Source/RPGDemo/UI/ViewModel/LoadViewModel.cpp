// Copyright 2024 Itmwuma


#include "UI/ViewModel/LoadViewModel.h"

#include "Level/Load/LoadManager.h"

int32 ULoadViewModel::ConvertSaveStatusToSwitchIndex(ESaveSlotStatus InStatus)
{
	return static_cast<int32>(InStatus);
}

void ULoadViewModel::BroadcastInitialValues()
{
	ULoadManager* LoadMgr = ULoadManager::Get(this);
	if (!IsValid(LoadMgr))
		return;
	
	for (int32 i = 0; i < ULoadManager::TotalSlotCount; ++i)
	{
		URPGSaveGame* SaveGameObject = LoadMgr->LoadSlotData(i);
		if (IsValid(SaveGameObject))
		{
			SetLoadDataInternal(SaveGameObject);
			OnLoadSwitcherIndexChanged.Broadcast(i, ConvertSaveStatusToSwitchIndex(ESaveSlotStatus::Taken));
			OnSaveSwitcherIndexChanged.Broadcast(i, ConvertSaveStatusToSwitchIndex(ESaveSlotStatus::Taken));
		}
		else
		{
			OnLoadSwitcherIndexChanged.Broadcast(i, ConvertSaveStatusToSwitchIndex(ESaveSlotStatus::Empty));
			OnSaveSwitcherIndexChanged.Broadcast(i, ConvertSaveStatusToSwitchIndex(ESaveSlotStatus::Empty));
		}
	}
}

void ULoadViewModel::LoadGameBySlot(int32 SlotIndex)
{
	if (ULoadManager* LoadMgr = ULoadManager::Get(this))
	{
		URPGSaveGame* SaveGameObject = LoadMgr->LoadSlotData(SlotIndex);
		if (IsValid(SaveGameObject))
		{
			LoadMgr->LoadLevelFromSaveGame(SaveGameObject);
		}
	}
}

void ULoadViewModel::DeleteSaveDataBySlot(int32 SlotIndex)
{
	if (ULoadManager* LoadMgr = ULoadManager::Get(this))
	{
		LoadMgr->DeleteSaveSlotData(SlotIndex);
		OnLoadSwitcherIndexChanged.Broadcast(SlotIndex, ConvertSaveStatusToSwitchIndex(ESaveSlotStatus::Empty));
		OnSaveSwitcherIndexChanged.Broadcast(SlotIndex, ConvertSaveStatusToSwitchIndex(ESaveSlotStatus::Empty));
	}
}

void ULoadViewModel::SaveGameBySlot(int32 SlotIndex)
{
	if (ULoadManager* LoadMgr = ULoadManager::Get(this))
	{
		URPGSaveGame* SaveGameObject = LoadMgr->SaveSlotData(SlotIndex);
		if (IsValid(SaveGameObject))
        {
			SetLoadDataInternal(SaveGameObject);
			OnSaveSwitcherIndexChanged.Broadcast(SaveGameObject->SlotIndex, ConvertSaveStatusToSwitchIndex(ESaveSlotStatus::Taken));
        }
	}
}

void ULoadViewModel::SetLoadDataInternal(const URPGSaveGame* SaveGameObject) const
{
	FSaveGameWidgetData SaveGameWidgetData;
	SaveGameWidgetData.PlayerName = FText::FromString(SaveGameObject->PlayerName);
	SaveGameWidgetData.SaveDate = FText::AsDateTime(SaveGameObject->SlotDateTime, EDateTimeStyle::Short);
	OnSaveDataChanged.Broadcast(SaveGameObject->SlotIndex, SaveGameWidgetData);
}
