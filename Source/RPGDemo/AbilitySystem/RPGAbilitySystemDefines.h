// Copyright 2024 Itmwuma

#pragma once

#include "Combat/AbilitySystem/ITMAbilityDefines.h"
#include "RPGAbilitySystemDefines.generated.h"

// 快速定义EEC计算所需参数
#define EEC_EXECUTE_INFO(ExecutionParams) \
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent(); \
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent(); \
	const AITMCharacterBase* SourceAvatar = SourceASC ? Cast<AITMCharacterBase>(SourceASC->GetAvatarActor()) : nullptr; \
	const AITMCharacterBase* TargetAvatar = TargetASC ? Cast<AITMCharacterBase>(TargetASC->GetAvatarActor()) : nullptr; \
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec(); \
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags(); \
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags(); \
	int32 SourceLevel = SourceAvatar ? SourceAvatar->GetCharacterLevel() : 1; \
	int32 TargetLevel = TargetAvatar ? TargetAvatar->GetCharacterLevel() : 1; \
	FAggregatorEvaluateParameters EvaluateParameters; \
	EvaluateParameters.SourceTags = SourceTags; \
	EvaluateParameters.TargetTags = TargetTags; \

USTRUCT()
struct FRPGDamageGameplayEffectContext : public FDamageGameplayEffectContext
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
	
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FGameplayEffectContext* Duplicate() const override;
};