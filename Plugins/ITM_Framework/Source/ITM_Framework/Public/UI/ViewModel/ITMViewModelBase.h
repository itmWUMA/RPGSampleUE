// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "ITMViewModelBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FITMViewModelParams
{
	GENERATED_BODY()

	FITMViewModelParams() { }
	FITMViewModelParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AttributeSet)
		: PlayerControllerCache(PC), PlayerStateCache(PS), ASCCache(ASC), AttributeSetCache(AttributeSet) { }

	FITMViewModelParams& operator=(const FITMViewModelParams& InParams);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerControllerCache = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerStateCache = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> ASCCache = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSetCache = nullptr;
};

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMViewModelBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInit();
	virtual void OnEnd();

	UFUNCTION(BlueprintCallable, Category = "UI|ViewModel")
	void SetViewModelParams(const FITMViewModelParams& InParams);

	UFUNCTION(BlueprintPure, Category = "UI|ViewModel")
	const FITMViewModelParams& GetViewModelParams() const { return Params; }

protected:
	virtual void BroadcastInitialValues() { }
	virtual void BindCallbackToDependencies() { }
	virtual void UnbindCallbackToDependencies() { }

protected:
	UPROPERTY(BlueprintReadOnly)
	FITMViewModelParams Params;
};
