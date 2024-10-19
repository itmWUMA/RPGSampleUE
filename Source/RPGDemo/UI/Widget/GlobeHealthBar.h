// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "GlobeHealthBar.generated.h"

class UProgressBar;
class UImage;
class USizeBox;

USTRUCT()
struct FSlowBarConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Units = "s", ClampMin = 0.1f, UIMin = 0.1f))
	float DelayTime = 1.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.001f, UIMin = 0.001f))
	float InterpSpeed = 1.f;
};

/**
 * 
 */
UCLASS()
class RPGDEMO_API UGlobeHealthBar : public UITMWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void InitWidgetsStyle() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void OnViewModelInitialized_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void SetPercent(float InValue);

private:
	void UpdateSlowBar(float InDeltaTime);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Image_Bg;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Main;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Slow;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Image_Glass;

	UPROPERTY(EditAnywhere, Category = "UI|Settings", meta = (ToolTip = "长+宽"))
	FVector2D WidgetSize = FVector2D(50.f, 50.f);

	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	FSlateBrush BgBrush;

	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	FSlateBrush MainBarFillBrush;

	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	FSlateBrush SlowBarFillBrush;

	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	FMargin ProgressBarPadding = FMargin(2.f);

	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	FSlateBrush GlassBrush;
	
	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	FSlowBarConfig SlowBarConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Logic")
	float SlowBarTargetPercent = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Logic")
	float SlowBarCurDelayTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Logic")
	bool bUpdatingSlowBar = false;
};
