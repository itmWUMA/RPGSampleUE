// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/Projectile/ITMLaunchObject.h"
#include "RPGProjectile.generated.h"

class UITMAttackComponent;
/**
 * 
 */
UCLASS()
class RPGDEMO_API ARPGProjectile : public AITMLaunchObject
{
	GENERATED_BODY()

public:
	ARPGProjectile();
	virtual void OnLaunchObjectRegistered() override;
	UITMAttackComponent* GetAttackComponent() { return AttackComp; }
	void SetAttackData(const FAttackData& Data);

protected:
	virtual void OnLaunchObjectOverlapped(AActor* TargetActor, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITMAttackComponent> AttackComp;

	FAttackData AttackData;
};
