// Copyright 2024 Itmwuma


#include "Level/Dither/DitherComponent.h"

#include "Kismet/KismetMaterialLibrary.h"

UDitherComponent::UDitherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDitherComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FadeFlag != 0)
	{
		const float TargetValue = (FadeFlag & 0x1) ? FadeOutTargetValue : 1.f;
		if (!FMath::IsNearlyEqual(TargetValue, CurFadeValue))
		{
			const float DesiredValue = FMath::FInterpTo(CurFadeValue, TargetValue, DeltaTime, FadeSpeed);
			SetDitherMatParam(DesiredValue);
			CurFadeValue = DesiredValue;
			if (FMath::IsNearlyEqual(TargetValue, CurFadeValue))
			{
				// 若恢复，则重新设置回原始的材质
				if (FadeFlag & 0x10)
				{
					for (int32 i = 0; i < OriginMatsCache.Num(); ++i)
					{
						SMCompCache->SetMaterial(i, OriginMatsCache[i]);
					}
				}
				FadeFlag = 0;
			}
		}
	}
}

void UDitherComponent::FadeOut()
{
	if (!FMath::IsNearlyEqual(FadeOutTargetValue, CurFadeValue))
	{
		for (int32 i = 0; i < DynamicMatsCache.Num(); ++i)
		{
			SMCompCache->SetMaterial(i, DynamicMatsCache[i]);
		}

		FadeFlag = 0x1;
	}
}

void UDitherComponent::FadeIn()
{
	if (!FMath::IsNearlyEqual(1.f, CurFadeValue))
		FadeFlag = 0x10;
}

void UDitherComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
		return;

	SMCompCache = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
	if (!SMCompCache.IsValid())
		return;

	const TArray<UMaterialInterface*>& OriginMats = SMCompCache->GetMaterials();
	OriginMatsCache = OriginMats;

	for (int32 i = 0; i < DitherMats.Num(); ++i)
	{
		UMaterialInstanceDynamic* DitherMID = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DitherMats[i]);
		DynamicMatsCache.Push(DitherMID);
	}

	CurFadeValue = 1.f;
}

void UDitherComponent::SetDitherMatParam(float Value)
{
	if (!SMCompCache.IsValid())
		return;
	
	for (UMaterialInstanceDynamic* DitherMID : DynamicMatsCache)
	{
		DitherMID->SetScalarParameterValue(DitherParamName, Value);
	}
}
