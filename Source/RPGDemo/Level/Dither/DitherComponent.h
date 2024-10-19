// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DitherComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGDEMO_API UDitherComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDitherComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Effect|Dither")
	void FadeOut();

	UFUNCTION(BlueprintCallable, Category = "Effect|Dither")
	void FadeIn();

protected:
	virtual void BeginPlay() override;

private:
	void SetDitherMatParam(float Value);

protected:
	UPROPERTY(EditAnywhere)
	FName DitherParamName = TEXT("Fade");

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> DitherMats;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.f, ClampMin = 0.f))
	float FadeSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	float FadeOutTargetValue = .2f;
	
	UPROPERTY(Transient)
	TArray<UMaterialInterface*> OriginMatsCache;

	UPROPERTY(Transient)
	TArray<UMaterialInstanceDynamic*> DynamicMatsCache;

private:
	uint8 FadeFlag = 0x0;	// 第一位表示FadeOut，第二位表示FadeIn
	float CurFadeValue = 1.f;;

	TWeakObjectPtr<UStaticMeshComponent> SMCompCache = nullptr;
};
