// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ITMViewModelBase.h"
#include "RootHUDViewModel.generated.h"

class UITMWidgetBase;

USTRUCT(BlueprintType)
struct FTagUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UITMWidgetBase> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagMessageInvoked, const FTagUIWidgetRow&, RowData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnShowDamageText, FVector, SorcePos, float, DamageValue, bool, bIsBlockedHit, bool, bIsCriticalHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPPercentChanged, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerLevelUpgrade, bool, bInit, int32, OldLevel, int32, NewLevel);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPGDEMO_API URootHUDViewModel : public UITMViewModelBase
{
	GENERATED_BODY()

public:
	virtual void OnInit() override;

protected:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;
	virtual void UnbindCallbackToDependencies() override;

private:
	void OnInitAttributeValues();
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnManaChanged(const FOnAttributeChangeData& Data);
	void OnMaxManaChanged(const FOnAttributeChangeData& Data);
	void OnAssetTagsApplied(const FGameplayTagContainer& TagContainer);
	void OnCharacterApplyDamage(AActor* Target, float DamageValue, bool bIsBlockedHit, bool bIsCriticalHit);
	void OnXPChanged(int32 OldXP, int32 NewXP);
	void OnPlayerLevelChanged(int32 OldLevel, int32 NewLevel);

	float GetXPPercent(int32 InXP) const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Tags", meta = (RequiredAssetDataTags  = "RowStructure=/Script/RPGDemo.TagUIWidgetRow"))
	TObjectPtr<UDataTable> MessageDataTable;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedDelegate OnHealthChangedDg;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedDelegate OnMaxHealthChangedDg;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedDelegate OnManaChangedDg;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedDelegate OnMaxManaChangedDg;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Tags")
	FOnTagMessageInvoked OnTagMessageInvokedDg;
 
	UPROPERTY(BlueprintAssignable, Category = "GAS|Damage")
	FOnShowDamageText OnShowDamageTextDg;

	UPROPERTY(BlueprintAssignable, Category = "XP")
	FOnXPPercentChanged OnXPPercentChangedDg;

	UPROPERTY(BlueprintAssignable, Category = "XP")
	FOnPlayerLevelUpgrade OnPlayerLevelUpgradeDg;

private:
	TMap<FGameplayTag, FName> MessageDataTableTagMap;

	FDelegateHandle OnCharacterHurtHandle;
};
