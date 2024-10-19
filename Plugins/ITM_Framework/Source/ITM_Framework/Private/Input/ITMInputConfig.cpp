// Copyright 2024 Itmwuma


#include "Input/ITMInputConfig.h"

const UInputAction* UITMInputConfig::GetInputAction(const FGameplayTag& InputTag) const
{
	if (const auto* Result = AbilityInputActions.Find(InputTag))
		return Result->InputAction;
	return nullptr;
}
