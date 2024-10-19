// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/NoExportTypes.h"
#include "ITMGameplayAbilityComponent.generated.h"

class UITMGameplayAbility;
/**
 * 
 */
UCLASS(Abstract, EditInlineNew)
class ITM_FRAMEWORK_API UITMGameplayAbilityComponent : public UObject
{
	GENERATED_BODY()

public:
	virtual void InitComponent(UITMGameplayAbility* InOwnerAbility);
	virtual void OnBegin(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo) { }
	virtual void OnEnd(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo) { }

protected:
	TWeakObjectPtr<UITMGameplayAbility> OwnerAbility = nullptr;
};
