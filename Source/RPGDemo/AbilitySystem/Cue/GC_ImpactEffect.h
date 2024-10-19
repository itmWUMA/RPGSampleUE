// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "Combat/AbilitySystem/ITMAbilityDefines.h"
#include "GC_ImpactEffect.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UGC_ImpactEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FArtisticEffectData ImpactEffect;
};
