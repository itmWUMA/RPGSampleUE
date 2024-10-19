// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectileAimPolicy.generated.h"

class AITMLaunchObject;
/**
 * 
 */
UCLASS(Abstract, EditInlineNew)
class RPGDEMO_API UProjectileAimPolicy : public UObject
{
	GENERATED_BODY()

public:
	virtual FRotator GetAimRotation(const AActor* InstigatorActor, const FTransform& BaseTransform) const;
};

UCLASS(DisplayName = "Closest Target")
class RPGDEMO_API UProjectileAimPolicy_ClosestTarget : public UProjectileAimPolicy
{
	GENERATED_BODY()

public:
	virtual FRotator GetAimRotation(const AActor* InstigatorActor, const FTransform& BaseTransform) const override;

protected:
	UPROPERTY(EditAnywhere)
	FRotator RotationOffset = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere)
	bool bIgnorePitch = false;
};

UCLASS(DisplayName = "Actor Direction")
class RPGDEMO_API UProjectileAimPolicy_ActorDirection : public UProjectileAimPolicy
{
	GENERATED_BODY()

public:
	virtual FRotator GetAimRotation(const AActor* InstigatorActor, const FTransform& BaseTransform) const override;

protected:
	UPROPERTY(EditAnywhere)
	FRotator RotationOffset = FRotator::ZeroRotator;
};