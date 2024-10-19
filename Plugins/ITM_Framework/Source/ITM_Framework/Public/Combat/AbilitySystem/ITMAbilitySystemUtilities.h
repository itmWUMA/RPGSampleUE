// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"

/**
 * 
 */
struct ITM_FRAMEWORK_API FITMAbilitySystemUtilities
{
	/**
	 * 根据传入公式，生成MMC计算结果
	 */
    static float CalcMMC(
    	UGameplayModMagnitudeCalculation* MMC,
    	const FGameplayEffectAttributeCaptureDefinition& Def,
    	const FGameplayEffectSpec& Spec,
    	TFunction<float(float, float)> CalcFunc);

	/**
	 * 获取当前能力的InputTag
	 */
	static FGameplayTag GetCurrentInputTag(const FGameplayAbilitySpec& Spec);

	/**
	 * 创建自定义EffectContext
	 */
	template<typename T>
	static T* AllocEffectContext()
	{
		static_assert(TIsDerivedFrom<T, FGameplayEffectContext>::IsDerived,
			"T can only be derived from FGameplayEffectContext");
		return new T();
	}
};
