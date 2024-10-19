// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_HitDetection.generated.h"

class UHitDetectTracePolicy;

UENUM()
enum class EHitDetectPositionType
{
	Actor,
	Socket,
	Weapon,
};

USTRUCT()
struct FHitDetectionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Detect Start Point")
	EHitDetectPositionType StartPositionType = EHitDetectPositionType::Actor;

	UPROPERTY(EditAnywhere, Category = "Detect Start Point", meta = (EditCondition = "StartPositionType==EHitDetectPositionType::Socket||StartPositionType==EHitDetectPositionType::Weapon", EditConditionHides))
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Detect Start Point")
	FVector StartPositionOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Detect Start Point")
	FRotator StartPointRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Instanced, Category = "Trace")
	TObjectPtr<UHitDetectTracePolicy> TracePolicy;

	UPROPERTY(EditAnywhere, Category = "Trace")
	FGameplayTag HitEventTag = FGameplayTag::EmptyTag;
};

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UANS_HitDetection : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	static void GetTraceStartPoint(const USkeletalMeshComponent* MeshComp, const FHitDetectionData& DetectionData, FVector& OutStartPoint, FRotator& OutStartRotation);
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<FHitDetectionData> HitDetectionDataList;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceType;
};
