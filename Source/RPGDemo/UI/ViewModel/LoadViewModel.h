// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Level/Load/RPGSaveGame.h"
#include "UI/ViewModel/ITMViewModelBase.h"
#include "LoadViewModel.generated.h"

USTRUCT(BlueprintType)
struct FSaveGameWidgetData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 Level = 0;

	UPROPERTY(BlueprintReadOnly)
	FText PlayerName = FText();

	UPROPERTY(BlueprintReadOnly)
	FText SaveDate = FText();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadSwitcherIndexChanged, int32, SlotIndex, int32, SwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveDataChanged, int32, SlotIndex, const FSaveGameWidgetData&, SaveData);

/**
 * 
 */
UCLASS(BlueprintType)
class RPGDEMO_API ULoadViewModel : public UITMViewModelBase
{
	GENERATED_BODY()

public:
	static int32 ConvertSaveStatusToSwitchIndex(ESaveSlotStatus InStatus);
	
protected:
	virtual void BroadcastInitialValues() override;

	UFUNCTION(BlueprintCallable)
	void LoadGameBySlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void DeleteSaveDataBySlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SaveGameBySlot(int32 SlotIndex);

private:
	void SetLoadDataInternal(const URPGSaveGame* SaveGameObject) const;

protected:
	UPROPERTY(BlueprintAssignable)
	FOnLoadSwitcherIndexChanged OnLoadSwitcherIndexChanged;

	UPROPERTY(BlueprintAssignable)
	FOnLoadSwitcherIndexChanged OnSaveSwitcherIndexChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSaveDataChanged OnSaveDataChanged;
};
