// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/Ability/Component/ITMGameplayAbilityComponent.h"
#include "AbilityComponent_PlayMontage.generated.h"

class UAbilityTask_PlayMontageAndWait;

UCLASS(Blueprintable, EditInlineNew)
class UMontageSelectCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	UAnimMontage* SelectMontage(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const TArray<UAnimMontage*>& InMontageCandidate) const;
	virtual UAnimMontage* SelectMontage_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const TArray<UAnimMontage*>& InMontageCandidate) const;
};

UCLASS()
class UMontageSelectCondition_Average : public UMontageSelectCondition
{
	GENERATED_BODY()

public:
	virtual UAnimMontage* SelectMontage_Implementation(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo, const TArray<UAnimMontage*>& InMontageCandidate) const override;
};

/**
 * 
 */
UCLASS(DisplayName = "Play Montage")
class ITM_FRAMEWORK_API UAbilityComponent_PlayMontage : public UITMGameplayAbilityComponent
{
	GENERATED_BODY()

public:
	virtual void OnBegin(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo) override;
	virtual void OnEnd(const FGameplayAbilitySpecHandle& Handle, const FGameplayAbilityActorInfo& ActorInfo) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnMontagePlayEnd();
	virtual void OnMontagePlayEnd_Implementation();
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AnimAssetCandidate;

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UMontageSelectCondition> MontageSelectCondition;

	UPROPERTY(EditAnywhere)
	bool bEndAbilityWhenPlayEnd = true;

private:
	UPROPERTY(Transient)
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = nullptr;
};
