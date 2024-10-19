// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Character/ITMPlayerCharacterBase.h"
#include "RPGPlayerCharacter.generated.h"

class UITMWeaponComponent;
/**
 * 
 */
UCLASS()
class RPGDEMO_API ARPGPlayerCharacter : public AITMPlayerCharacterBase
{
	GENERATED_BODY()

public:
	ARPGPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	virtual void InitHUD() override;
	virtual void InitAbilityActorInfo() override;

private:
	void OnLevelChanged(int32 OldLevel, int32 NewLevel);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITMWeaponComponent> WeaponComp;

	UPROPERTY(EditDefaultsOnly, Category = "LevelUp")
	FArtisticEffectData LevelUpEffectData;
};
