// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/RootHUDViewModel.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "MessageWidget.generated.h"

class UTextBlock;
class UImage;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class RPGDEMO_API UMessageWidget : public UITMWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|MessageWidget")
	void SetMessageInfo(const FTagUIWidgetRow& Data);

	UFUNCTION(BlueprintPure, Category ="UI|MessageWidget")
	bool ShouldDestroyWidget() const { return bPendingDestroy; }

protected:
	virtual void InitWidgetsStyle() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Root;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_Show;

	UPROPERTY(BlueprintReadWrite, Category = "Logic")
	bool bPendingDestroy = false;
};
