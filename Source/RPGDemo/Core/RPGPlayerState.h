// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Core/ITMPlayerState.h"
#include "AbilitySystem/XP/LevelUpInfoDataAsset.h"
#include "RPGPlayerState.generated.h"

class URPGSaveGame;
class URPGAttributeSetBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnXPChanged, int32 OldXP, int32 NewXP);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributePointChanged, int32 OldAttributePoint, int32 NewAttributePoint);

/**
 * 
 */
UCLASS()
class RPGDEMO_API ARPGPlayerState : public AITMPlayerState
{
	GENERATED_BODY()

public:
	ARPGPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// --- IITMAbilitySystemInterface impl ---
	virtual UAttributeSet* GetAttributeSet() const override;
	// --- IITMAbilitySystemInterface impl end ---

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|XP")
	void SetXP(int32 InXP);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|XP")
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintPure, Category = "AbilitySystem|XP")
	int32 GetXP() const { return XP; }

	UFUNCTION(BlueprintPure, Category = "AbilitySystem|XP")
	int32 GetAttributePoint() const { return AttributePoint; }

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|XP")
	void AddToAttributePoint(int32 InAttributePoint);

	FORCEINLINE ULevelUpInfoDataAsset* GetLevelUpInfo() const { return LevelUpInfo; }
	FORCEINLINE FOnXPChanged& GetOnXPChangedDelegate() { return OnXPChanged; }
	FORCEINLINE FOnAttributePointChanged& GetOnAttributePointChanged() { return OnAttributePointChanged; }

	void InitPlayerStateInfoFromSaveGame(const URPGSaveGame* InSaveGame);

private:
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem|Attribute")
	TObjectPtr<URPGAttributeSetBase> AttributeSet;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem|XP", ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem|XP",  ReplicatedUsing=OnRep_AttributePoint)
	int32 AttributePoint = 0;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem|XP")
	TObjectPtr<ULevelUpInfoDataAsset> LevelUpInfo;

	FOnXPChanged OnXPChanged;
	FOnAttributePointChanged OnAttributePointChanged;
};
