// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGGameplayStatics.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API URPGGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GAS|Effect", meta = (WorldContext = "WorldContext"))
	static const UCurveTable* GetEffectCoefficients(const UObject* WorldContext);
};
