// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Ability/Component/AbilityComponent_PlayMontage.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"

UAnimMontage* UMontageSelectCondition::SelectMontage_Implementation(const FGameplayAbilitySpecHandle& Handle,
	const FGameplayAbilityActorInfo& ActorInfo, const TArray<UAnimMontage*>& InMontageCandidate) const
{
	return InMontageCandidate.IsEmpty() ? nullptr : InMontageCandidate[0];
}

UAnimMontage* UMontageSelectCondition_Average::SelectMontage_Implementation(const FGameplayAbilitySpecHandle& Handle,
	const FGameplayAbilityActorInfo& ActorInfo, const TArray<UAnimMontage*>& InMontageCandidate) const
{
	if (InMontageCandidate.IsEmpty())
		return nullptr;

	const int32 FinalIndex = FMath::RandRange(0, InMontageCandidate.Num() - 1);
	return InMontageCandidate[FinalIndex];
}

void UAbilityComponent_PlayMontage::OnBegin(const FGameplayAbilitySpecHandle& Handle,
                                            const FGameplayAbilityActorInfo& ActorInfo)
{
	if (AnimAssetCandidate.IsEmpty() || !OwnerAbility.IsValid())
		return;

	UAnimMontage* FinalMontage = IsValid(MontageSelectCondition) ?
		MontageSelectCondition->SelectMontage(Handle, ActorInfo, AnimAssetCandidate) : AnimAssetCandidate[0];
	if (!IsValid(FinalMontage))
		return;

	PlayMontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(OwnerAbility.Get(), NAME_None, FinalMontage);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UAbilityComponent_PlayMontage::OnMontagePlayEnd);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UAbilityComponent_PlayMontage::OnMontagePlayEnd);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UAbilityComponent_PlayMontage::OnMontagePlayEnd);
	
	PlayMontageTask->Activate();
}

void UAbilityComponent_PlayMontage::OnEnd(const FGameplayAbilitySpecHandle& Handle,
	const FGameplayAbilityActorInfo& ActorInfo)
{
	if (PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.RemoveAll(this);
		PlayMontageTask->OnInterrupted.RemoveAll(this);
		PlayMontageTask->OnCancelled.RemoveAll(this);
		PlayMontageTask = nullptr;
	}
	
	Super::OnEnd(Handle, ActorInfo);
}

void UAbilityComponent_PlayMontage::OnMontagePlayEnd_Implementation()
{
	if (bEndAbilityWhenPlayEnd && OwnerAbility.IsValid())
	{
		OwnerAbility->TryEndAbility();
	}
}
