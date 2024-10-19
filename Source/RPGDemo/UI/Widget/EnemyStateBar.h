// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "EnemyStateBar.generated.h"

class UEnemyStateViewModel;
class UProgressBar;
class USizeBox;
/**
 * 
 */
UCLASS()
class RPGDEMO_API UEnemyStateBar : public UITMWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void InitWidgetsStyle() override;
	virtual void OnViewModelInitialized_Implementation() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnHealthChange(float NewValue);

	UFUNCTION()
	void OnMaxHealthChange(float NewValue);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_StateBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Settings")
	FVector2D WidgetSize = FVector2D(50.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Settings")
	FProgressBarStyle StateBarStyle;

private:
	TWeakObjectPtr<UEnemyStateViewModel> EnemyStateViewModelCache = nullptr;
};
