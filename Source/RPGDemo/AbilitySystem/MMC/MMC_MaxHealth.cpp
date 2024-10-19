// Copyright 2024 Itmwuma


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Combat/AbilitySystem/ITMAbilitySystemUtilities.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = URPGAttributeSetBase::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	auto CalcFunc = [](float Magnitude, float Level) -> float
	{
		Magnitude = FMath::Max(Magnitude, 0.f);
		return 80.f + 2.5f * Magnitude + 10.f * Level;
	};
	
	return FITMAbilitySystemUtilities::CalcMMC(const_cast<UMMC_MaxHealth*>(this), VigorDef, Spec, CalcFunc);
}
