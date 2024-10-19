// Copyright 2024 Itmwuma


#include "AbilitySystem/RPGAttackComponent.h"

#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"

void URPGAttackComponent::MakeAttackContext(const FAttackData& InAttackData,
                                            FGameplayEffectContextHandle& ContextHandle)
{
	ContextHandle = ASCCache->MakeDamageEffectContext(InAttackData);
}
