// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Components/ActorComponent.h"
#include "AwardComponent.generated.h"

class UGameplayEffect;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGDEMO_API UAwardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAwardComponent();

	UFUNCTION(BlueprintPure, Category = "AbilitySystem|XP")
	int32 GetReward(float InLevel) const;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|XP")
	void GiveReward(AActor* Target, float InLevel);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> RewardGE;
	
	UPROPERTY(EditAnywhere)
	FScalableFloat XPReward;
};
