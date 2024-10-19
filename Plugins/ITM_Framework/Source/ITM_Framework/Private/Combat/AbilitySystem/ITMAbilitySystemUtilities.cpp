// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/ITMAbilitySystemUtilities.h"

#include "GameplayModMagnitudeCalculation.h"
#include "Character/ITMCharacterBase.h"
#include "Settings/ITMFrameworkSettings.h"

float FITMAbilitySystemUtilities::CalcMMC(UGameplayModMagnitudeCalculation* MMC,
                                          const FGameplayEffectAttributeCaptureDefinition& Def, const FGameplayEffectSpec& Spec,
                                          TFunction<float(float, float)> CalcFunc)
{
	if (!MMC)
		return 0.f;
	
	// 收集SourceTags和TargetTags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 获取值
	float Magnitude = 0.f;
	UFunction* GetMagnitudeFunc = UGameplayModMagnitudeCalculation::StaticClass()->FindFunctionByName(TEXT("K2_GetCapturedAttributeMagnitude"));
	if (GetMagnitudeFunc)
	{
		struct FGetCapturedAttributeMagnitudeFuncParams
		{
			FGameplayEffectSpec Spec;
			FGameplayAttribute Attribute;
			FGameplayTagContainer SourceTags;
			FGameplayTagContainer TargetTags;
			float Magnitude;
		};
		
		FGetCapturedAttributeMagnitudeFuncParams FuncParams{Spec, Def.AttributeToCapture, *SourceTags, *TargetTags, Magnitude};
		MMC->ProcessEvent(GetMagnitudeFunc, &FuncParams);

		// 获取等级
		float Level = 1.f;
		if (const AITMCharacterBase* SourceCharacter = Cast<AITMCharacterBase>(Spec.GetContext().GetSourceObject()))
		{
			Level = SourceCharacter->GetCharacterLevel();
		}

		return CalcFunc(FuncParams.Magnitude, Level);
	}

	return 0.f;
}

FGameplayTag FITMAbilitySystemUtilities::GetCurrentInputTag(const FGameplayAbilitySpec& Spec)
{
	const FGameplayTagContainer FilterInputTags =
		Spec.DynamicAbilityTags.Filter(UITMFrameworkSettings::Get()->DefaultInputBaseTag.GetSingleTagContainer());
	return FilterInputTags.First();
}
