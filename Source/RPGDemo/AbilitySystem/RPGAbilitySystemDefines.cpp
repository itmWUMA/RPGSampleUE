// Copyright 2024 Itmwuma

#include "RPGAbilitySystemDefines.h"

UScriptStruct* FRPGDamageGameplayEffectContext::GetScriptStruct() const
{
	return FDamageGameplayEffectContext::GetScriptStruct();
}

FGameplayEffectContext* FRPGDamageGameplayEffectContext::Duplicate() const
{
	FRPGDamageGameplayEffectContext* NewContext = new FRPGDamageGameplayEffectContext();
	*NewContext = *this;
	NewContext->AddActors(Actors);
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}
