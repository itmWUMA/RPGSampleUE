// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/ITMAbilityDefines.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"
#include "AttackGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UAttackGameplayAbility : public UITMGameplayAbility
{
	GENERATED_BODY()

public:
	const FAttackData& GetAttackData() const { return AttackData; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FAttackData AttackData;
};
