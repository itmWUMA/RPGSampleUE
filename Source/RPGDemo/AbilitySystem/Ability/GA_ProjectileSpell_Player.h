// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/GA_ProjectileSpell.h"
#include "GA_ProjectileSpell_Player.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UGA_ProjectileSpell_Player : public UGA_ProjectileSpell
{
	GENERATED_BODY()

protected:
	virtual FTransform GetSpawnTransform(const FGameplayAbilityActorInfo& ActorInfo) const override;
	virtual FRotator GetFacingTargetRotation(const FGameplayAbilityActorInfo& ActorInfo) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Projectile", Instanced)
	TObjectPtr<UProjectileAimPolicy> SkillModifyAimPolicy;
};
