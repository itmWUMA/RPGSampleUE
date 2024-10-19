// Copyright 2024 Itmwuma


#include "UI/ITMUserInterfaceUtilities.h"

#include "Core/ITMPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/ITMWidgetBase.h"

void FITMUserInterfaceUtilities::HideWidget(UITMWidgetBase* Widget, bool bShouldShowMouseCursor)
{
	if (!Widget)
		return;

	Widget->SetVisibility(ESlateVisibility::Collapsed);
	Widget->SetWidgetInputMode(bShouldShowMouseCursor);
	if (UITMViewModelBase* ViewModel = Widget->GetViewModel())
	{
		ViewModel->OnEnd();
	}
}

void FITMUserInterfaceUtilities::ResumeWidget(UITMWidgetBase* Widget)
{
	if (!Widget)
		return;

	Widget->SetVisibility(ESlateVisibility::Visible);
	Widget->SetWidgetInputMode(true);
	if (UITMViewModelBase* ViewModel = Widget->GetViewModel())
	{
		ViewModel->OnInit();
	}
}

FITMViewModelParams FITMUserInterfaceUtilities::MakeDefaultViewModelParams(const UObject* WorldContext)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, 0))
	{
		AITMPlayerState* PS = PC->GetPlayerState<AITMPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AttributeSet = PS->GetAttributeSet();
		return FITMViewModelParams{PC, PS, ASC, AttributeSet};
	}
	return FITMViewModelParams();
}
