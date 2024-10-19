// Copyright 2024 Itmwuma


#include "Combat/Notify/ANS_HitDetection.h"
#include "Combat/Notify/HitTracePolicy/HitDetectTracePolicy.h"
#include "Combat/Weapon/ITMWeaponComponent.h"

void UANS_HitDetection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	for (const FHitDetectionData& HitDetectionData : HitDetectionDataList)
	{
		if (HitDetectionData.TracePolicy)
		{
			FVector StartPoint = FVector::ZeroVector;
			FRotator StartRotation = FRotator::ZeroRotator;
			GetTraceStartPoint(MeshComp, HitDetectionData, StartPoint, StartRotation);
			HitDetectionData.TracePolicy->StartTrace(MeshComp, TraceType, StartPoint, StartRotation, HitDetectionData.HitEventTag);
		}
	}
}

void UANS_HitDetection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	for (const FHitDetectionData& HitDetectionData : HitDetectionDataList)
	{
		if (HitDetectionData.TracePolicy)
		{
			HitDetectionData.TracePolicy->EndTrace(MeshComp);
		}
	}
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UANS_HitDetection::GetTraceStartPoint(const USkeletalMeshComponent* MeshComp, const FHitDetectionData& DetectionData, FVector& OutStartPoint,
                                           FRotator& OutStartRotation)
{
	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner()))
		return;
	
	FTransform BaseTransform = FTransform::Identity;
	
	switch (DetectionData.StartPositionType)
	{
	case EHitDetectPositionType::Actor:
		BaseTransform = MeshComp->GetOwner()->GetActorTransform();
		break;
	case EHitDetectPositionType::Socket:
		BaseTransform = MeshComp->GetSocketTransform(DetectionData.SocketName);
		break;
	case EHitDetectPositionType::Weapon:
		{
			if (const UITMWeaponComponent* WeaponComp = MeshComp->GetOwner()->GetComponentByClass<UITMWeaponComponent>();
				IsValid(WeaponComp) && IsValid(WeaponComp ->GetWeaponMesh()))
			{
				BaseTransform = WeaponComp->GetWeaponMesh()->GetSocketTransform(DetectionData.SocketName);
			}
		}
		break;
	default:
		checkNoEntry();
	}

	OutStartRotation = BaseTransform.TransformRotation(DetectionData.StartPointRotation.Quaternion()).Rotator();
	BaseTransform.SetRotation(OutStartRotation.Quaternion());
	OutStartPoint = BaseTransform.TransformPosition(DetectionData.StartPositionOffset);
}
