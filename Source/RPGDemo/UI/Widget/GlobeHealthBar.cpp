// Copyright 2024 Itmwuma


#include "UI/Widget/GlobeHealthBar.h"

#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"

void UGlobeHealthBar::InitWidgetsStyle()
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetHeightOverride(WidgetSize.X);
		SizeBox_Root->SetWidthOverride(WidgetSize.Y);
	}

	if (Image_Bg)
	{
		if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image_Bg->Slot))
		{
			OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
			OverlaySlot->SetVerticalAlignment(VAlign_Fill);
		}
		Image_Bg->SetBrush(BgBrush);
	}

	if (ProgressBar_Main)
	{
		if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(ProgressBar_Main->Slot))
		{
			OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
			OverlaySlot->SetVerticalAlignment(VAlign_Fill);
			OverlaySlot->SetPadding(ProgressBarPadding);
		}
		ProgressBar_Main->SetFillColorAndOpacity(FLinearColor::White);
		FProgressBarStyle NewStyle = ProgressBar_Main->GetWidgetStyle();
		NewStyle.FillImage = MainBarFillBrush;
		ProgressBar_Main->SetWidgetStyle(NewStyle);
	}
	
	if (ProgressBar_Slow)
	{
		if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(ProgressBar_Slow->Slot))
		{
			OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
			OverlaySlot->SetVerticalAlignment(VAlign_Fill);
			OverlaySlot->SetPadding(ProgressBarPadding);
		}
		ProgressBar_Slow->SetFillColorAndOpacity(FLinearColor::White);
		FProgressBarStyle NewStyle = ProgressBar_Slow->GetWidgetStyle();
		NewStyle.FillImage = SlowBarFillBrush;
		ProgressBar_Slow->SetWidgetStyle(NewStyle);
	}

	if (Image_Glass)
	{
		if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image_Glass->Slot))
		{
			OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
			OverlaySlot->SetVerticalAlignment(VAlign_Fill);
			OverlaySlot->SetPadding(ProgressBarPadding);
		}
		Image_Glass->SetBrush(GlassBrush);
	}
}

void UGlobeHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateSlowBar(InDeltaTime);
}

void UGlobeHealthBar::OnViewModelInitialized_Implementation()
{
	ProgressBar_Main->SetPercent(0.f);
	ProgressBar_Slow->SetPercent(0.f);
	
	//Super::OnViewModelInitialized_Implementation();
}

void UGlobeHealthBar::SetPercent(float InValue)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	ProgressBar_Main->SetPercent(InValue);

	// Slow条延时设置
	bUpdatingSlowBar = false;
	if (InValue < ProgressBar_Slow->GetPercent())
	{
		SlowBarCurDelayTime = SlowBarConfig.DelayTime;
		SlowBarTargetPercent = InValue;
	}
	else
	{
		ProgressBar_Slow->SetPercent(InValue);
	}
}

void UGlobeHealthBar::UpdateSlowBar(float InDeltaTime)
{
	if (SlowBarCurDelayTime > 0.f)
	{
		SlowBarCurDelayTime -= InDeltaTime;
		if (SlowBarCurDelayTime <= 0.f)
		{
			bUpdatingSlowBar = true;
		}
	}

	if (bUpdatingSlowBar)
	{
		const float CurSlowBarPercent = ProgressBar_Slow->GetPercent();
		if (CurSlowBarPercent == SlowBarTargetPercent)
		{
			bUpdatingSlowBar = false;
		}
		else
		{
			const float DesiredPercent = FMath::FInterpConstantTo(
				CurSlowBarPercent, SlowBarTargetPercent, InDeltaTime, SlowBarConfig.InterpSpeed);
			ProgressBar_Slow->SetPercent(DesiredPercent);
		}
	}
}
