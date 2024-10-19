// Copyright 2024 Itmwuma


#include "UI/Widget/MessageWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMessageWidget::SetMessageInfo(const FTagUIWidgetRow& Data)
{
	Image_Icon->SetBrushFromSoftTexture(Data.Image);
	Text_Message->SetText(Data.Message);
	PlayAnimationForward(Anim_Show);
}

void UMessageWidget::InitWidgetsStyle()
{
	Super::InitWidgetsStyle();

	if (Image_Icon)
	{
		if (UHorizontalBoxSlot* ImageIconSlot = Cast<UHorizontalBoxSlot>(Image_Icon->Slot))
		{
			ImageIconSlot->SetHorizontalAlignment(HAlign_Left);
			ImageIconSlot->SetVerticalAlignment(VAlign_Center);
		}
	}

	if (Text_Message)
	{
		if (UHorizontalBoxSlot* TextMessageSlot = Cast<UHorizontalBoxSlot>(Text_Message->Slot))
		{
			TextMessageSlot->SetHorizontalAlignment(HAlign_Left);
			TextMessageSlot->SetVerticalAlignment(VAlign_Center);
		}
	}
}
