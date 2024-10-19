// Copyright 2024 Itmwuma


#include "AbilitySystem/EEC/EEC_AttributeCalculation.h"

#include "AbilitySystem/RPGAbilitySystemDefines.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Character/ITMCharacterBase.h"

namespace RPGCalcCapture
{
	struct FAttributeCalculationCaptureStatics
	{
		DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
		DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
		DECLARE_ATTRIBUTE_CAPTUREDEF(Resilience);
		DECLARE_ATTRIBUTE_CAPTUREDEF(Vigor);

		FAttributeCalculationCaptureStatics()
		{
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, Strength, Source, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, Intelligence, Source, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, Resilience, Source, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, Vigor, Source, false);
		}
	};

	static const FAttributeCalculationCaptureStatics& AttributeCalculationCaptureStatics()
	{
		static FAttributeCalculationCaptureStatics AttributeCalculationCaptureStatics;
		return AttributeCalculationCaptureStatics;
	}
}

UEEC_AttributeCalculation::UEEC_AttributeCalculation()
{
	RelevantAttributesToCapture.Add(RPGCalcCapture::AttributeCalculationCaptureStatics().StrengthDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::AttributeCalculationCaptureStatics().IntelligenceDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::AttributeCalculationCaptureStatics().ResilienceDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::AttributeCalculationCaptureStatics().VigorDef);
}

void UEEC_AttributeCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	EEC_EXECUTE_INFO(ExecutionParams);

	// 捕获基础属性数值
	float Strength = 0.f, Intelligence = 0.f, Resilience = 0.f, Vigor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		RPGCalcCapture::AttributeCalculationCaptureStatics().StrengthDef,
		EvaluateParameters,
		Strength);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		RPGCalcCapture::AttributeCalculationCaptureStatics().IntelligenceDef,
		EvaluateParameters,
		Intelligence);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		RPGCalcCapture::AttributeCalculationCaptureStatics().ResilienceDef,
		EvaluateParameters,
		Resilience);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		RPGCalcCapture::AttributeCalculationCaptureStatics().VigorDef,
		EvaluateParameters,
		Vigor);

	// 计算次级属性
	const float Armor = 0.25f * (Resilience + 2.f) + 6.f;
	const float ArmorPenetration = 0.15f * (Resilience + 1.f) + 3.f;
	const float BlockChance = 0.25f * Armor + 4.f;
	const float CriticalHitChance = 0.25f * ArmorPenetration + 2.f;
	const float CriticalHitResilience = 0.25f * Armor + 10.f;
	const float CriticalHitDamage = 1.5f * ArmorPenetration + 5.f;
	const float HealthRegeneration = 0.1f * Vigor + 1.f;
	const float ManaRegeneration = 0.1f * Intelligence + 1.f;

	// 输出结果
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetArmorAttribute(),
		EGameplayModOp::Override,
		Armor));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetArmorPenetrationAttribute(),
		EGameplayModOp::Override,
		ArmorPenetration));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetBlockChanceAttribute(),
		EGameplayModOp::Override,
		BlockChance));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetCriticalHitChanceAttribute(),
		EGameplayModOp::Override,
		CriticalHitChance));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetCriticalHitChanceAttribute(),
		EGameplayModOp::Override,
		CriticalHitResilience));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetCriticalHitDamageAttribute(),
		EGameplayModOp::Override,
		CriticalHitDamage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetHealthRegenerationAttribute(),
		EGameplayModOp::Override,
		HealthRegeneration));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		URPGAttributeSetBase::GetManaRegenerationAttribute(),
		EGameplayModOp::Override,
		ManaRegeneration));
}
