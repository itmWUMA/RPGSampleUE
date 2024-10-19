// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ITMPlayerController.generated.h"

class UAbilityBindingComponent;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API AITMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AITMPlayerController();

	FORCEINLINE TObjectPtr<UAbilityBindingComponent> GetAbilityBindingComp() const { return AbilityBindingComp; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UAbilityBindingComponent> AbilityBindingComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;
};
