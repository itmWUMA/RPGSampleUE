// Copyright 2024 Itmwuma


#include "AbilitySystem/Entity/EntityTransformGenerator.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

FTransform UEntityTransformGenerator::GetTargetTransforms(const FEntityTransformGenerateData& GenerateData,
	int32 Index) const
{
	return FTransform::Identity;
}

FTransform UEntityTransformGenerator_SpawnInSphere::GetTargetTransforms(
	const FEntityTransformGenerateData& GenerateData, int32 Index) const
{
	if (!GenerateData.OwnerActor.IsValid())
		return Super::GetTargetTransforms(GenerateData, Index);

	FTransform OwnerTransform = GenerateData.OwnerActor->GetActorTransform();
	FVector SphereCenter = OwnerTransform.TransformPosition(CirclePointOffset);
	FVector TargetPoint = SphereCenter + UKismetMathLibrary::RandomUnitVector() * Radius;

#if WITH_EDITOR
	if (bDebug)
	{
		UKismetSystemLibrary::DrawDebugSphere(
			GenerateData.OwnerActor.Get(),
			SphereCenter,
			Radius,
			12,
			FLinearColor::Blue,
			2.f,
			1.f);
		UKismetSystemLibrary::DrawDebugPoint(
			GenerateData.OwnerActor.Get(),
			TargetPoint,
			5.f,
			FLinearColor::Green,
			2.f);
	}
#endif

	return FTransform(GenerateData.OwnerActor->GetActorQuat(), TargetPoint);
}
