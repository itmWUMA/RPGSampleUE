// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "AbilityInputBindingObject.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ITMInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FITMInputActionData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UAbilityInputBindingObject> AbilityBindingObject;
};

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* GetInputAction(const FGameplayTag& InputTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FITMInputActionData> AbilityInputActions;
};
