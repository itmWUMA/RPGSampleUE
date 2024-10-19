// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterDeath.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterDeath : public UInterface
{
	GENERATED_BODY()
};

class RPGDEMO_API ICharacterDeath
{
	GENERATED_BODY()

public:
	virtual void OnDead() = 0;
	virtual bool IsDead() const = 0;
	virtual void StartDie() { }
};
