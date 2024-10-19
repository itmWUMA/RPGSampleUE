// Copyright 2024 Itmwuma


#include "AbilitySystem/XP/AwardComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/RPGGameplayTagDefines.h"

UAwardComponent::UAwardComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UAwardComponent::GetReward(float InLevel) const
{
	return static_cast<int32>(XPReward.GetValueAtLevel(InLevel));
}

void UAwardComponent::GiveReward(AActor* Target, float InLevel)
{
	if (!IsValid(Target) && !IsValid(GetOwner()))
		return;
  
	const IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(Target);
	if (!TargetASI)
		return;
	
	UAbilitySystemComponent* SourceASC = GetOwner()->GetComponentByClass<UAbilitySystemComponent>();
	UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent();
	
	if (SourceASC && TargetASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(RewardGE, InLevel, EffectContextHandle);
		int32 PendingReward = GetReward(InLevel);
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(Name_GameplayTag_Reward), static_cast<float>(PendingReward));
		SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}
}

