// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EEC_Damage.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UEEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UEEC_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
