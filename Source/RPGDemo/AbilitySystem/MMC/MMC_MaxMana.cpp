// Copyright 2024 Itmwuma


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Combat/AbilitySystem/ITMAbilitySystemUtilities.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = URPGAttributeSetBase::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	auto CalcFunc = [](float Magnitude, float Level) -> float
	{
		Magnitude = FMath::Max(Magnitude, 0.f);
		return 50.f + 2.5f * Magnitude + 15.f * Level;
	};
	
	return FITMAbilitySystemUtilities::CalcMMC(const_cast<UMMC_MaxMana*>(this), IntelligenceDef, Spec, CalcFunc);
}
