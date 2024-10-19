// Copyright 2024 Itmwuma


#include "UI/Widget/ITMWidgetBase.h"

#include "Kismet/GameplayStatics.h"

void UITMWidgetBase::SetViewModel(UITMViewModelBase* InViewModel)
{
	ViewModel = InViewModel;
	OnViewModelInitialized();
}

void UITMWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitWidgetsStyle();
}

void UITMWidgetBase::SetWidgetInputMode(bool bShouldShowMouseCursor)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
		return;

	const bool bShowMouseCursorResult = bShouldShowMouseCursor ? bShowMouseCursor : bShouldShowMouseCursor;
	PC->bShowMouseCursor = bShowMouseCursorResult;

	if (bShowMouseCursorResult)
	{
		if (bDisableGameplayInput)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			PC->SetInputMode(InputMode);
		}
		else
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			PC->SetInputMode(InputMode);
		}
	}
	else
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}
