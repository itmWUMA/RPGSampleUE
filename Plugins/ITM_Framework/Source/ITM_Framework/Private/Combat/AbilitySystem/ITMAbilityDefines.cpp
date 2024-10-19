// Copyright 2024 Itmwuma

#include "Combat/AbilitySystem/ITMAbilityDefines.h"

float FAttackData::GetDamageValue(int32 InLevel) const
{
	return DamageValue.GetValueAtLevel(InLevel);
}

UScriptStruct* FDamageGameplayEffectContext::GetScriptStruct() const
{
	return FGameplayEffectContext::GetScriptStruct();
}

FGameplayEffectContext* FDamageGameplayEffectContext::Duplicate() const
{
	FDamageGameplayEffectContext* NewContext = new FDamageGameplayEffectContext();
	*NewContext = *this;
	NewContext->AddActors(Actors);
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}
