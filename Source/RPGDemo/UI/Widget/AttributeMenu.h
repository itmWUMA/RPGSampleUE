// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "AttributeMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UAttributeMenu : public UITMWidgetBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Logic")
	void CloseAttributeMenu();

	virtual void OnViewModelInitialized_Implementation() override;
};
