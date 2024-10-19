// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Component/ITMAttackComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Settings/ITMFrameworkSettings.h"

void UITMAttackComponent::Init(UITMAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	ASCCache = ASC;
	AttackAbilitySpecHandle = AbilitySpecHandle;
}

void UITMAttackComponent::Attack(AActor* Target, const FAttackData& InAttackData)
{
	if (!ASCCache.IsValid() ||
		!DamageEffectClass)
		return;

	const FGameplayAbilitySpec* AttackAbilitySpec = ASCCache->FindAbilitySpecFromHandle(AttackAbilitySpecHandle);
	const int32 DamageLevel = AttackAbilitySpec ? AttackAbilitySpec->Level : 1;
	FGameplayEffectContextHandle ContextHandle;
	MakeAttackContext(InAttackData, ContextHandle);
	DamageEffectSpecHandle = ASCCache->MakeOutgoingSpec(DamageEffectClass, DamageLevel, ContextHandle);

	if (const UITMFrameworkSettings* Settings = UITMFrameworkSettings::Get())
	{
		DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(Settings->DamageTag, InAttackData.GetDamageValue(DamageLevel));
	}
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
	{
		ASCCache->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), TargetASC);
	}
}

void UITMAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UITMAttackComponent::MakeAttackContext(const FAttackData& InAttackData,
	FGameplayEffectContextHandle& ContextHandle)
{
	ContextHandle = ASCCache->MakeDamageEffectContext(InAttackData);
}
