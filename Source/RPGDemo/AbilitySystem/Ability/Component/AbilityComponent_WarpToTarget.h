// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/Ability/Component/ITMGameplayAbilityComponent.h"
#include "AbilityComponent_WarpToTarget.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Warp To Target")
class RPGDEMO_API UAbilityComponent_WarpToTarget : public UITMGameplayAbilityComponent
{
	GENERATED_BODY()

public:
	virtual void OnBegin(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo) override;

protected:
	UPROPERTY(EditAnywhere)
	FName WarpTargetName = TEXT("FacingTarget");
};

UCLASS(DisplayName = "Warp To Player")
class RPGDEMO_API UAbilityComponent_WarpToPlayer : public UITMGameplayAbilityComponent
{
	GENERATED_BODY()

public:
	virtual void OnBegin(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo) override;

protected:
	UPROPERTY(EditAnywhere)
	FName WarpTargetName = TEXT("FacingTarget");
};