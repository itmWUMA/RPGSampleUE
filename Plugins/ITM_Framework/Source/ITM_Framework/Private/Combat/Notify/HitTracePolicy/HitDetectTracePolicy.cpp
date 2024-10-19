// Copyright 2024 Itmwuma


#include "Combat/Notify/HitTracePolicy/HitDetectTracePolicy.h"

#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Combat/AbilitySystem/Component/ITMAttackComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UHitDetectTracePolicy::SetHitResultToInstigator(const USkeletalMeshComponent* MeshComp,
	const FHitResult& HitResult)
{
	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner()))
		return;

	if (UITMAttackComponent* AttackComp = MeshComp->GetOwner()->GetComponentByClass<UITMAttackComponent>())
	{
		AttackComp->SetCurrentHitResult(HitResult);
	}
}

void UHitDetectTracePolicy_InstantTrace::StartTrace(USkeletalMeshComponent* MeshComp,
                                                    ETraceTypeQuery CollisionChannel, const FVector& StartPoint, const FRotator& StartRotation, const FGameplayTag& HitEventTag)
{
	if (!IsValid(MeshComp))
		return;
	
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	bool bTraceResult = false;
	EDrawDebugTrace::Type DebugTraceType = EDrawDebugTrace::None;

#if WITH_EDITOR
	if (bDebug)
		DebugTraceType = EDrawDebugTrace::ForDuration;
#endif
	
	switch (TraceType)
	{
	case EHitDetectTraceType::Ray:
		bTraceResult = UKismetSystemLibrary::LineTraceSingle(
			MeshComp,
			StartPoint,
			StartPoint + StartRotation.Vector() * TraceValue.X,
			CollisionChannel,
			bTraceComplex,
			ActorsToIgnore,
			DebugTraceType,
			HitResult,
			true);
		break;
	case EHitDetectTraceType::Box:
		bTraceResult = UKismetSystemLibrary::BoxTraceSingle(
			MeshComp,
			StartPoint,
			StartPoint,
			TraceValue,
			StartRotation,
			CollisionChannel,
			bTraceComplex,
			ActorsToIgnore,
			DebugTraceType,
			HitResult,
			true);
		break;
	case EHitDetectTraceType::Sphere:
		bTraceResult = UKismetSystemLibrary::SphereTraceSingle(
			MeshComp,
			StartPoint,
			StartPoint,
			TraceValue.X,
			CollisionChannel,
			bTraceComplex,
			ActorsToIgnore,
			DebugTraceType,
			HitResult,
			true);
		break;
	case EHitDetectTraceType::Capsule:
		bTraceResult = UKismetSystemLibrary::CapsuleTraceSingle(
			MeshComp,
			StartPoint,
			StartPoint,
			TraceValue.Y,
			TraceValue.X,
			CollisionChannel,
			bTraceComplex,
			ActorsToIgnore,
			DebugTraceType,
			HitResult,
			true);
		break;
	default:
		checkNoEntry();
	}

	if (bTraceResult)
	{
		if (const IITMAbilitySystemInterface* ASI = Cast<IITMAbilitySystemInterface>(MeshComp->GetOwner()))
		{
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
			{
				FGameplayEventData Payload;
				Payload.Instigator = MeshComp->GetOwner();
				Payload.Target = HitResult.GetActor();
				SetHitResultToInstigator(MeshComp, HitResult);
				ASC->HandleGameplayEvent(HitEventTag, &Payload);
			}
		}
	}
}
