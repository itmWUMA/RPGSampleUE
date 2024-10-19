// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ITMViewModelBase.h"
#include "AttributeMenuViewModel.generated.h"

USTRUCT(BlueprintType)
struct FAttributeShowInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeDescription = FText();
};

UCLASS(BlueprintType)
class RPGDEMO_API UAttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	bool GetAttributeInfo(const FGameplayAttribute& Attribute, FAttributeShowInfo& OutInfo) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttributeShowInfo> ListenAttributeInfo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListenedAttributeChanged, const FAttributeShowInfo&, AttributeShowInfo, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributePointChanged, int32, NewAttributePoint);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPGDEMO_API UAttributeMenuViewModel : public UITMViewModelBase
{
	GENERATED_BODY()

protected:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;
	virtual void UnbindCallbackToDependencies() override;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|XP")
	void UpgradeAttribute(const FGameplayAttribute& InAttribute);

private:
	void OnListenedAttributeChanged(const FOnAttributeChangeData& Data);
	void OnAttributeChanged(int32 OldAttributePoint, int32 NewAttributePoint);

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Attributes")
	TObjectPtr<UAttributeInfoDataAsset> AttributeShowInfo;

protected:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnListenedAttributeChanged OnListenedAttributeChangedDg;

	UPROPERTY(BlueprintAssignable, Category = "XP")
	FOnPlayerAttributePointChanged OnAttributePointChangedDg;

private:
	TMap<FGameplayAttribute, FDelegateHandle> ListenedAttributeDelegateHandle;
};
