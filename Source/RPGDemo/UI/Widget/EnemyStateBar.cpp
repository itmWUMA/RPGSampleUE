// Copyright 2024 Itmwuma


#include "UI/Widget/EnemyStateBar.h"

#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/ViewModel/EnemyStateViewModel.h"

void UEnemyStateBar::InitWidgetsStyle()
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetHeightOverride(WidgetSize.X);
		SizeBox_Root->SetWidthOverride(WidgetSize.Y);
	}

	if (ProgressBar_StateBar)
	{
		ProgressBar_StateBar->SetWidgetStyle(StateBarStyle);
	}
}

void UEnemyStateBar::OnViewModelInitialized_Implementation()
{
	EnemyStateViewModelCache = Cast<UEnemyStateViewModel>(ViewModel);
	if (EnemyStateViewModelCache.IsValid())
	{
		EnemyStateViewModelCache->OnHealthChanged.AddDynamic(this, &UEnemyStateBar::OnHealthChange);
		EnemyStateViewModelCache->OnMaxHealthChanged.AddDynamic(this, &UEnemyStateBar::OnMaxHealthChange);
	}
}

void UEnemyStateBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (EnemyStateViewModelCache.IsValid())
	{
		const bool bShowWidget = EnemyStateViewModelCache->CanShowWidget();
		SetVisibility(bShowWidget ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		if (bShowWidget)
		{
			const float DesiredScale = EnemyStateViewModelCache->GetDesiredWidgetScale();
			SizeBox_Root->SetRenderScale(FVector2D(DesiredScale));
		}
	}
}

void UEnemyStateBar::OnHealthChange(float NewValue)
{
	if (EnemyStateViewModelCache.IsValid())
	{
		ProgressBar_StateBar->SetPercent(UKismetMathLibrary::SafeDivide(NewValue, EnemyStateViewModelCache->GetCurHealthValue().Y));
	}
}

void UEnemyStateBar::OnMaxHealthChange(float NewValue)
{
	if (EnemyStateViewModelCache.IsValid())
	{
		ProgressBar_StateBar->SetPercent(UKismetMathLibrary::SafeDivide(EnemyStateViewModelCache->GetCurHealthValue().X, NewValue));
	}
}
