// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EntityTransformGenerator.generated.h"

struct FEntityTransformGenerateData
{
	TWeakObjectPtr<AActor> OwnerActor;
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class RPGDEMO_API UEntityTransformGenerator : public UObject
{
	GENERATED_BODY()

public:
	virtual FTransform GetTargetTransforms(const FEntityTransformGenerateData& GenerateData, int32 Index) const;

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	bool bDebug = false;
#endif
};

UCLASS(DisplayName = "Spawn In Sphere")
class UEntityTransformGenerator_SpawnInSphere : public UEntityTransformGenerator
{
	GENERATED_BODY()

public:
	virtual FTransform GetTargetTransforms(const FEntityTransformGenerateData& GenerateData, int32 Index) const override;
	
public:
	UPROPERTY(EditAnywhere)
	float Radius = 100.f;

	UPROPERTY(EditAnywhere)
	FVector CirclePointOffset = FVector::ZeroVector;
	
};
