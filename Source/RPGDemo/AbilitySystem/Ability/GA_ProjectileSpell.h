// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/Ability/AttackGameplayAbility.h"
#include "GA_ProjectileSpell.generated.h"

class UProjectileAimPolicy;
class AITMLaunchObject;

UENUM()
enum class EProjectileSpawnPositionType : uint8
{
	CharacterMesh,
	CharacterSocket,
	WeaponSocket,
};

/**
 * 
 */
UCLASS()
class RPGDEMO_API UGA_ProjectileSpell : public UAttackGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Ability")
	AITMLaunchObject* SpawnProjectile();

	UFUNCTION(BlueprintPure, Category = "GAS|Ability")
	virtual FTransform GetSpawnTransform(const FGameplayAbilityActorInfo& ActorInfo) const;

	UFUNCTION(BlueprintPure, Category = "GAS|Ability")
	virtual FRotator GetFacingTargetRotation(const FGameplayAbilityActorInfo& ActorInfo) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AITMLaunchObject> LaunchObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	EProjectileSpawnPositionType SpawnPositionType = EProjectileSpawnPositionType::WeaponSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (EditCondition = "SpawnPositionType != EProjectileSpawnPositionType::CharacterMesh", EditConditionHides))
	FName SocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", Instanced)
	TObjectPtr<UProjectileAimPolicy> AimPolicy;
};
