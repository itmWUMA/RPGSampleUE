// Copyright 2024 Itmwuma


#include "AbilitySystem/Projectile/ProjectileAimPolicy.h"

#include "Level/Interaction/InteractionManager.h"

FRotator UProjectileAimPolicy::GetAimRotation(const AActor* InstigatorActor,
                                              const FTransform& BaseTransform) const
{
	return BaseTransform.Rotator();
}

FRotator UProjectileAimPolicy_ClosestTarget::GetAimRotation(const AActor* InstigatorActor,
	const FTransform& BaseTransform) const
{
	FRotator TargetRotation = BaseTransform.Rotator();
	if (!InstigatorActor || !InstigatorActor->GetWorld())
		return TargetRotation;

	if (const UInteractionManager* InteractionMgr = InstigatorActor->GetWorld()->GetSubsystem<UInteractionManager>())
	{
		if (const AActor* Target = InteractionMgr->GetClosestHighlightedObject(InstigatorActor))
		{
			// 查找到目标，则朝向目标方向
			TargetRotation = (Target->GetActorLocation() - BaseTransform.GetLocation()).Rotation();
		}
		else
		{
			// 未查找到目标，则朝向角色面向方向
			TargetRotation = InstigatorActor->GetActorRotation();
		}
	}

	TargetRotation = TargetRotation + RotationOffset;

	if (bIgnorePitch)
		TargetRotation.Pitch = 0.f;
	
	return TargetRotation;
}

FRotator UProjectileAimPolicy_ActorDirection::GetAimRotation(const AActor* InstigatorActor,
	const FTransform& BaseTransform) const
{
	if (!InstigatorActor)
		return Super::GetAimRotation(InstigatorActor, BaseTransform);

	return InstigatorActor->GetActorRotation() + RotationOffset;
}
