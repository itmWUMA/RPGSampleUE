// Copyright 2024 Itmwuma


#include "UI/Widget/LoadWidget.h"

#include "Components/PanelWidget.h"

void ULoadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (UWidget* Widget : SlotsPanel->GetAllChildren())
	{
		if (UITMWidgetBase* ITMWidget = Cast<UITMWidgetBase>(Widget))
		{
			SlotsCache.Add(ITMWidget);
		}
	}
}

void ULoadWidget::NativeDestruct()
{
	SlotsCache.Empty();
	
	Super::NativeDestruct();
}

void ULoadWidget::OnViewModelInitialized_Implementation()
{
	for (UITMWidgetBase* Widget : SlotsCache)
	{
		Widget->SetViewModel(ViewModel);
	}
}
