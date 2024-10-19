// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "LoadWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API ULoadWidget : public UITMWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnViewModelInitialized_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UPanelWidget> SlotsPanel;
	
	UPROPERTY(BlueprintReadOnly, Category = "Load")
	TArray<UITMWidgetBase*> SlotsCache;
};
