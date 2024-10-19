// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/ITMAbilityDefines.h"
#include "GameFramework/Actor.h"
#include "ITMLaunchObject.generated.h"

class ULaunchObjectMovementComponent;


UCLASS(Abstract)
class ITM_FRAMEWORK_API AITMLaunchObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AITMLaunchObject();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnLaunchObjectRegistered();
	virtual void OnLaunchObjectLaunched();
	virtual void OnLaunchObjectDestroyed();
	virtual void OnLaunchObjectOverlapped(AActor* TargetActor, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Registered")
	void BP_OnLaunchObjectRegistered();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Launched")
	void BP_OnLaunchObjectLaunched();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Destroyed")
	void BP_OnLaunchObjectDestroyed();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Overlapped")
	void BP_OnLaunchObjectOverlapped(AActor* TargetActor, const FHitResult& SweepResult);

private:
	void RegisterOverlapComponents();
	bool CanOverlap(const AActor* TargetActor) const;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<ULaunchObjectMovementComponent> MovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FArtisticEffectData ImpactEffectData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyWhenOverlap = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> IgnoreActorsType;
	
	// 注册所有标记为Overlap检测的组件，用以发射物的碰撞检测
	UPROPERTY(Transient)
	TArray<UPrimitiveComponent*> OverlapComps;
};
