// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadManager.generated.h"

class URPGSaveGame;
class ULoadWidget;
class ULoadSystemData;
class APlayerStart;

/**
 * 
 */
UCLASS()
class RPGDEMO_API ULoadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Load")
	void ShowLoadUI();

	UFUNCTION(BlueprintCallable, Category = "Load")
	void CloseLoadUI();

	UFUNCTION(BlueprintCallable, Category = "Load")
	void ShowSaveUI();

	UFUNCTION(BlueprintCallable, Category = "Load")
	void CloseSaveUI();

	UFUNCTION(BlueprintCallable, Category = "Load")
	void CreateNewGame();

	UFUNCTION(BlueprintCallable, Category = "Load")
	void LoadLevelFromSaveGame(URPGSaveGame* InSaveGame);

	UFUNCTION(BlueprintPure, Category = "Load")
	int32 GetCurrentSaveSlotIndex() const { return CurrentSaveSlotIndex; }

	UFUNCTION(BlueprintPure, Category = "Load")
	FName GetDefaultPlayerStartTag() const;

	static ULoadManager* Get(const UObject* WorldContext);
	static FString MakeSlotNameByIndex(int32 Index);
	
	URPGSaveGame* SaveSlotData(int32 SlotIndex);
	URPGSaveGame* LoadSlotData(int32 SlotIndex);
	void DeleteSaveSlotData(int32 SlotIndex);

	void LoadActorInfoFromSaveGame(const URPGSaveGame* InSaveGame) const;

private:
	APlayerStart* GetClosestPlayerStart() const;

public:
	static constexpr int32 TotalSlotCount = 4;

private:
	UPROPERTY(Transient)
	TObjectPtr<ULoadSystemData> LoadDataAsset;

	UPROPERTY(Transient)
	TObjectPtr<ULoadWidget> LoadWidgetCache;

	UPROPERTY(Transient)
	TObjectPtr<ULoadWidget> SaveWidgetCache;

	UPROPERTY()
	int32 CurrentSaveSlotIndex = -1;
};
