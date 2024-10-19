// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Character/ITMCharacterBase.h"
#include "ITMPlayerCharacterBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ITM_FRAMEWORK_API AITMPlayerCharacterBase : public AITMCharacterBase
{
	GENERATED_BODY()

public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual int32 GetCharacterLevel() const override;

protected:
	virtual void InitAbilityActorInfo();
	virtual void InitHUD() {}
};
