// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Ability/HurtGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Combat/AbilitySystem/Component/ITMHurtComponent.h"

void UHurtGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo ||
		!ActorInfo->AvatarActor.IsValid() ||
		!ActorInfo->AbilitySystemComponent.IsValid() ||
		!HurtEffectDataTable)
		return;

	const TWeakObjectPtr<UAbilitySystemComponent> ASC = ActorInfo->AbilitySystemComponent;
	const FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HurtEffectClass, GetAbilityLevel(), Context);
	HurtEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	if (const UITMHurtComponent* HurtComponent = ActorInfo->AvatarActor->GetComponentByClass<UITMHurtComponent>())
	{
		const FHurtDataCollection& CurrentHurtData = HurtComponent->GetCurrentHurtData();
		if (const FHurtEffectDataTableRow* EffectDataTableRow = HurtEffectDataTable->FindRow<FHurtEffectDataTableRow>(AbilityTags.First().GetTagName(), TEXT("")))
		{
			if (const FHurtEffectData* HurtEffectData = EffectDataTableRow->HurtEffectData.Find(CurrentHurtData.HurtTypeTag))
			{
				ApplyHurtEffect(Handle, ASC.Get(), *HurtEffectData, CurrentHurtData);
			}
		}
	}
}

void UHurtGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (HurtEffectHandle.IsValid())
	{
		OnHurtAnimEnd();
	}

	if (HurtEffectTimerHandle.IsValid())
	{
		HurtEffectTimerHandle.Invalidate();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHurtGameplayAbility::ApplyHurtEffect(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* ASC,
                                           const FHurtEffectData& HurtEffectData, const FHurtDataCollection& HurtData)
{
	if (HurtEffectData.AnimAsset)
	{
		auto* PlayMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, HurtEffectData.AnimAsset);
		PlayMontageTask->OnCompleted.AddDynamic(this, &UHurtGameplayAbility::OnHurtAnimEnd);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UHurtGameplayAbility::OnHurtAnimEnd);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UHurtGameplayAbility::OnHurtAnimEnd);
		PlayMontageTask->Activate();
	}
	else
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(HurtEffectTimerHandle, [WeakThis = TWeakObjectPtr<UHurtGameplayAbility>(this)]
			{
				WeakThis->OnHurtAnimEnd();
			}, HurtEffectData.EffectDelayTime, false);
		}
	}
}

void UHurtGameplayAbility::OnHurtAnimEnd()
{
	if (!HurtEffectHandle.IsValid())
		return;

	if (TWeakObjectPtr<UAbilitySystemComponent> ASC = GetActorInfo().AbilitySystemComponent; ASC.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(HurtEffectHandle);
		HurtEffectHandle.Invalidate();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
