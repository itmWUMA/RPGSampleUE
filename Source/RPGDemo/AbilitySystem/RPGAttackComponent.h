// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/Component/ITMAttackComponent.h"
#include "RPGAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API URPGAttackComponent : public UITMAttackComponent
{
	GENERATED_BODY()

protected:
	virtual void MakeAttackContext(const FAttackData& InAttackData, FGameplayEffectContextHandle& ContextHandle) override;
};
