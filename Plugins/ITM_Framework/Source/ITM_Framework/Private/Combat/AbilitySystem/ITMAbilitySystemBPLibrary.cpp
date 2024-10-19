// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/ITMAbilitySystemBPLibrary.h"

#include "Combat/AbilitySystem/ITMAbilitySystemUtilities.h"

FGameplayTag UITMAbilitySystemBPLibrary::GetCurrentInputTag(const FGameplayAbilitySpec& Spec)
{
	return FITMAbilitySystemUtilities::GetCurrentInputTag(Spec);
}
