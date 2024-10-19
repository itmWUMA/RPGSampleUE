// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EEC_AttributeCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UEEC_AttributeCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UEEC_AttributeCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
