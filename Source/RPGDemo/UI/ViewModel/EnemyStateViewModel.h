// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "RootHUDViewModel.h"
#include "UI/ViewModel/ITMViewModelBase.h"
#include "EnemyStateViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyHealthReduce);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPGDEMO_API UEnemyStateViewModel : public UITMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "EnemyStateWidget")
	float GetDesiredWidgetScale() const;

	UFUNCTION(BlueprintPure, Category = "EnemyStateWidget")
	FVector2D GetCurHealthValue() const { return {CurHealth, CurMaxHealth}; }

	UFUNCTION(BlueprintPure, Category = "EnemyStateWidget")
	bool CanShowWidget() const;

protected:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedDelegate OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyHealthReduce OnEnemyHealthReduce;

	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	TObjectPtr<UCurveFloat> WidgetScaleCurve;

private:
	float CurHealth = 0.f;
	float CurMaxHealth = 0.f;
};
