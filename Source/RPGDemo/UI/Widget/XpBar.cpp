// Copyright 2024 Itmwuma


#include "UI/Widget/XpBar.h"

#include "Components/ProgressBar.h"
#include "UI/ViewModel/RootHUDViewModel.h"

void UXpBar::OnViewModelInitialized_Implementation()
{
	Super::OnViewModelInitialized_Implementation();

	if (URootHUDViewModel* HUDViewModel = Cast<URootHUDViewModel>(ViewModel))
	{
		HUDViewModel->OnXPPercentChangedDg.AddDynamic(this,&UXpBar::OnXPPercentChange);
	}
}

void UXpBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bChangingPercent)
		return;
	
	float DesiredValue = CurValue;
	if (TargetValue >= CurValue)
	{
		DesiredValue = FMath::FInterpConstantTo(CurValue, TargetValue, InDeltaTime, InterpSpeed);
		ProgressBar_XP->SetPercent(DesiredValue);
		CurValue = DesiredValue;
		if (FMath::IsNearlyEqual(CurValue, TargetValue))
		{
			bChangingPercent = false;
		}
	}
	else
	{
		DesiredValue = FMath::FInterpConstantTo(CurValue, 1.f, InDeltaTime, InterpSpeed);
		ProgressBar_XP->SetPercent(DesiredValue);
		CurValue = DesiredValue;
		if (FMath::IsNearlyEqual(CurValue, 1.f))
		{
			CurValue = 0.f;
		}
	}
}

void UXpBar::OnXPPercentChange(float Percent)
{
	TargetValue = Percent;
	bChangingPercent = true;
}
