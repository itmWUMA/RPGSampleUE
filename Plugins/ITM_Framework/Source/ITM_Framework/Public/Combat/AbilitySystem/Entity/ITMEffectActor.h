// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "ITMEffectActor.generated.h"

class UGameplayEffect;
class UITMAbilitySystemComponent;

UCLASS()
class ITM_FRAMEWORK_API AITMEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AITMEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Effect")
	bool ApplyEffectToTarget(AActor* EffectTarget);

	UFUNCTION(BlueprintCallable, Category = "GAS|Effect")
	void RemoveEffect(AActor* EffectTarget);

	UFUNCTION(BlueprintNativeEvent, Category = "GAS|Effect")
	bool CanApplyEffect(AActor* PendingTarget);
	virtual bool CanApplyEffect_Implementation(AActor* PendingTarget);

protected:
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> GEClass;

	UPROPERTY(EditAnywhere, Category = "Effects", meta = (ClampMin = 0.f, UIMin = 0.f))
	float Level = 1;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TArray<FName> IgnoreTags;

private:
	TMap<TWeakObjectPtr<AActor>, FActiveGameplayEffectHandle> EffectHandles;
};
