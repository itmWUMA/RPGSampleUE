// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViewModel/ITMViewModelBase.h"
#include "ITMUIBlueprintFunctionLibrary.generated.h"

class UITMViewModelBase;

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMUIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "UI|ViewModel", meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "ViewModelId"))
	static UITMViewModelBase* GetViewModel(const UObject* WorldContext, const FName& ViewModelId);

	UFUNCTION(BlueprintPure, Category = "UI|ViewModel", meta = (WorldContext = "WorldContext"))
	static FITMViewModelParams MakeDefaultViewModelParams(const UObject* WorldContext);
};
