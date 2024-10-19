// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "ITMUserInterfaceDefines.generated.h"

class UITMViewModelBase;
class UITMWidgetBase;

USTRUCT()
struct FITMWidgetClassConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UITMWidgetBase> WidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UITMViewModelBase> ViewModelClass;
};
