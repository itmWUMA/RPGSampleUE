// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/RPGGameplayAbility.h"
#include "GA_Summon.generated.h"

class UNiagaraSystem;
class UEntityTransformGenerator;

/**
 * 
 */
UCLASS()
class RPGDEMO_API UGA_Summon : public URPGGameplayAbility
{
	GENERATED_BODY()

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|Summon")
	void StartSummon();
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "AbilitySystem|Summon")
	TArray<FVector> GetSpawnLocations() const;
	virtual TArray<FVector> GetSpawnLocations_Implementation() const;

private:
	void SummonActorInternal(const FVector& TargetLocation, const ACharacter* PlayerCharacter, AActor* AvatarActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Summon", meta = (ClampMin = 1, UIMin = 1))
	int32 SpawnPointCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Summon")
	TSubclassOf<AActor> SummonedClass;

	UPROPERTY(EditAnywhere, Instanced, Category = "Summon")
	TObjectPtr<UEntityTransformGenerator> TransformGenerator;

	UPROPERTY(EditAnywhere, Category = "Summon", meta = (ClampMin = 0.f, UIMin = 0.f))
	float SummonInterval = 0.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	bool bCanSummonWhenSummonedExist = false;

	UPROPERTY(EditAnywhere, Category = "Summon")
	bool bSummonOnGround = true;

	UPROPERTY(EditAnywhere, Category = "Summon", meta = (EditCondition = "bSummonOnGround", EditConditionHides))
	TEnumAsByte<ECollisionChannel> LineTraceChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditAnywhere, Category = "Summon", meta = (EditCondition = "bSummonOnGround", EditConditionHides))
	float TraceDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Summon")
	TObjectPtr<UNiagaraSystem> SummonParticle;

private:
	TArray<FTimerHandle> SummonTimerHandle;
	TArray<TWeakObjectPtr<AActor>> SummonedActorCache;
};
