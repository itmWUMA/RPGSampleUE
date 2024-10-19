// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "HitDetectTracePolicy.generated.h"

UENUM()
enum class EHitDetectTraceType
{
	Ray,
	Box,
	Sphere,
	Capsule,
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class ITM_FRAMEWORK_API UHitDetectTracePolicy : public UObject
{
	GENERATED_BODY()

public:
	virtual void StartTrace(
		USkeletalMeshComponent* MeshComp,
		ETraceTypeQuery CollisionChannel,
		const FVector& StartPoint,
		const FRotator& StartRotation,
		const FGameplayTag& HitEventTag) { }
	virtual void EndTrace(USkeletalMeshComponent* MeshComp) { }

protected:
	static void SetHitResultToInstigator(const USkeletalMeshComponent* MeshComp, const FHitResult& HitResult);
};

UCLASS(DisplayName = "Instant Trace")
class UHitDetectTracePolicy_InstantTrace : public UHitDetectTracePolicy
{
	GENERATED_BODY()

public:
	virtual void StartTrace(USkeletalMeshComponent* MeshComp, ETraceTypeQuery CollisionChannel, const FVector& StartPoint, const FRotator& StartRotation, const FGameplayTag& HitEventTag) override;

protected:
	UPROPERTY(EditAnywhere)
	EHitDetectTraceType TraceType = EHitDetectTraceType::Ray;

	/**
	 * 射线检测：X表示检测长度
	 * 盒体检测：XYZ表示盒体的Extend
	 * 球体检测：X表示球体半径
	 * 胶囊体检测：X表示胶囊体半高，Y表示胶囊体半径
	 */
	UPROPERTY(EditAnywhere)
	FVector TraceValue = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	bool bTraceComplex = false;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	bool bDebug = false;
#endif
};
