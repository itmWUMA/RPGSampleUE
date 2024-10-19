// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "XpBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class RPGDEMO_API UXpBar : public UITMWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void OnViewModelInitialized_Implementation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnXPPercentChange(float Percent);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpSpeed = 1.f;

private:
	float TargetValue = 0.f;
	float CurValue = 0.f;
	bool bChangingPercent = false;
};
