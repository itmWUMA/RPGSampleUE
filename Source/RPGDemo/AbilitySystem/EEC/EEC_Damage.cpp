// Copyright 2024 Itmwuma


#include "AbilitySystem/EEC/EEC_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/RPGAbilitySystemDefines.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Character/ITMCharacterBase.h"
#include "Core/RPGGameplayStatics.h"
#include "Settings/ITMFrameworkSettings.h"

namespace RPGCalcCapture
{
	struct FRPGDamageExecutionCaptureStatics
	{
		DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
		DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
		DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
		DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
		DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
		DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
		
		FRPGDamageExecutionCaptureStatics()
		{
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, Armor, Target, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, ArmorPenetration, Source, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, BlockChance, Target, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, CriticalHitChance, Source, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, CriticalHitResistance, Target, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase, CriticalHitDamage, Source, false);
		}
	};

	static const FRPGDamageExecutionCaptureStatics& DamageExecutionCaptureStatics()
	{
		static FRPGDamageExecutionCaptureStatics DamageExecutionCaptureStatics;
		return DamageExecutionCaptureStatics;
	}
}

UEEC_Damage::UEEC_Damage()
{
	RelevantAttributesToCapture.Add(RPGCalcCapture::DamageExecutionCaptureStatics().ArmorDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::DamageExecutionCaptureStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::DamageExecutionCaptureStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::DamageExecutionCaptureStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::DamageExecutionCaptureStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(RPGCalcCapture::DamageExecutionCaptureStatics().CriticalHitDamageDef);
}

void UEEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	EEC_EXECUTE_INFO(ExecutionParams);
	FRPGDamageGameplayEffectContext* Context = static_cast<FRPGDamageGameplayEffectContext*>(EffectSpec.GetContext().Get());

	const UCurveTable* EffectCoefficients = URPGGameplayStatics::GetEffectCoefficients(TargetASC);
	float ArmorPenetrationCoefficient = .25f;
	float EffectiveArmorCoefficient = .33f;
	if (EffectCoefficients)
	{
		if (FRealCurve* ArmorPenetrationCurve = EffectCoefficients->FindCurve(TEXT("ArmorPenetration"), FString()))
			ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel, .25f);
		if (FRealCurve* EffectiveArmorCurve = EffectCoefficients->FindCurve(TEXT("EffectiveArmor"), FString()))
			EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetLevel, .33);
	}

	// 获取伤害值（来源AttackComponent）
	float Damage = 0.f;
	if (const UITMFrameworkSettings* Settings = UITMFrameworkSettings::Get())
	{
		Damage = EffectSpec.GetSetByCallerMagnitude(Settings->DamageTag);
	}
	
	bool bCriticalHit = false, bDamageBlocked = false;

	// 计算伤害格挡（成功格挡，伤害减半）
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		RPGCalcCapture::DamageExecutionCaptureStatics().BlockChanceDef,
		EvaluateParameters,
		BlockChance);
	BlockChance = FMath::Max(BlockChance, 0.f);
	bDamageBlocked = FMath::RandRange(1, 100) < BlockChance;
	Damage = bDamageBlocked ? Damage * .5f :Damage;

	if (!bDamageBlocked)
	{
		// 护甲和护甲穿透修正伤害值
		float Armor = 0, ArmorPenetration = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			RPGCalcCapture::DamageExecutionCaptureStatics().ArmorDef,
			EvaluateParameters,
			Armor);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			RPGCalcCapture::DamageExecutionCaptureStatics().ArmorPenetrationDef,
			EvaluateParameters,
			ArmorPenetration);
		Armor = FMath::Max(Armor, 0.f);
		ArmorPenetration = FMath::Clamp(ArmorPenetration, 0.f, 100.f * ArmorPenetrationCoefficient);
		float EffectiveArmor = Armor * (100.f - ArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
		EffectiveArmor = FMath::Clamp(EffectiveArmor, 0.f, 100.f * EffectiveArmorCoefficient);
		Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

		// 暴击伤害修正
		float CriticalHitChance = 0.f, CriticalHitResistance = 0.f, CriticalHitDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			RPGCalcCapture::DamageExecutionCaptureStatics().CriticalHitChanceDef,
			EvaluateParameters,
			CriticalHitChance);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			RPGCalcCapture::DamageExecutionCaptureStatics().CriticalHitResistanceDef,
			EvaluateParameters,
			CriticalHitResistance);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			RPGCalcCapture::DamageExecutionCaptureStatics().CriticalHitDamageDef,
			EvaluateParameters,
			CriticalHitDamage);
		CriticalHitChance = FMath::Max(CriticalHitChance, 0.f);
		CriticalHitResistance = FMath::Max(CriticalHitResistance, 0.f);
		CriticalHitDamage = FMath::Max(CriticalHitDamage, 0.f);
		float EffectiveCriticalHitChance = CriticalHitChance - CriticalHitResistance * .15f;
		bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
		Damage = bCriticalHit ? Damage * 2.f + CriticalHitDamage : Damage;
	}

	if (Context)
	{
		Context->bIsBlockedHit = bDamageBlocked;
		Context->bIsCriticalHit = bCriticalHit;
	}

	// 输出计算结果
	const FGameplayModifierEvaluatedData DamageEvaluatedData(
		URPGAttributeSetBase::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		Damage);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Damage Value : %f"), Damage));
	
	OutExecutionOutput.AddOutputModifier(DamageEvaluatedData);
}