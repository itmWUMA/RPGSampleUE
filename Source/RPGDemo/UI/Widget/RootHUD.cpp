// Copyright 2024 Itmwuma


#include "UI/Widget/RootHUD.h"

#include "GlobeHealthBar.h"
#include "MessageWidget.h"
#include "HurtTextWidget.h"
#include "XpBar.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void URootHUD::InitWidgetsStyle()
{
	Super::InitWidgetsStyle();

	InitGlobeBar(HealthBar);
	InitGlobeBar(ManaBar);

	if (MessageWidgetContainer)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MessageWidgetContainer->Slot))
		{
			CanvasSlot->SetAutoSize(true);
		}
	}
}

void URootHUD::OnViewModelInitialized_Implementation()
{
	Super::OnViewModelInitialized_Implementation();

	// 将RootHUD的ViewModel传入子蓝图中进行复用
	HealthBar->SetViewModel(ViewModel);
	ManaBar->SetViewModel(ViewModel);
	XpBar->SetViewModel(ViewModel);
}

UMessageWidget* URootHUD::CreateMessageWidget(const FTagUIWidgetRow& Data)
{
	// 检查并清除需要移除的MessageUI
	CheckAndRemoveMessageWidgets();
	
	const TSubclassOf<UMessageWidget> WidgetClass{ Data.MessageWidgetClass ? Data.MessageWidgetClass : DefaultMessageWidgetClass};
	UMessageWidget* MessageWidget = CreateWidget<UMessageWidget>(this, WidgetClass);
	if (!MessageWidget)
		return nullptr;

	UVerticalBoxSlot* WidgetSlot = MessageWidgetContainer->AddChildToVerticalBox(MessageWidget);
	WidgetSlot->SetHorizontalAlignment(HAlign_Left);
	WidgetSlot->SetVerticalAlignment(VAlign_Center);
	MessageWidget->SetMessageInfo(Data);
	
	return MessageWidget;
}

UHurtTextWidget* URootHUD::CreateHurtTextWidget(const FVector& SourcePos, float DamageValue, bool bBlockedHit, bool bCriticalHit)
{
	UHurtTextWidget* HurtTextWidget = CreateWidget<UHurtTextWidget>(this, HurtTextWidgetClass);
	if (!HurtTextWidget)
		return nullptr;
	
	UCanvasPanelSlot* WidgetSlot = Canvas_Root->AddChildToCanvas(HurtTextWidget);
	WidgetSlot->SetAlignment(FVector2D(.5f, .5f));
	HurtTextWidget->SetViewModel(ViewModel);
	HurtTextWidget->InitHurtTextWidget(DamageValue, SourcePos, bBlockedHit, bCriticalHit);

	return HurtTextWidget;
}

void URootHUD::InitGlobeBar(UGlobeHealthBar* InGlobeBar)
{
	if (!InGlobeBar)
		return;

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InGlobeBar->Slot))
	{
		CanvasSlot->SetAnchors(FAnchors(.5f, 1.f, .5f, 1.f));
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetAlignment(FVector2D(.5f, 1.f));
	}
}

void URootHUD::CheckAndRemoveMessageWidgets()
{
	bool bShouldRemoveMessageWidgets = true;
	for (UWidget* Widget : MessageWidgetContainer->GetAllChildren())
	{
		if (UMessageWidget* MessageWidget = Cast<UMessageWidget>(Widget))
		{
			if (!MessageWidget->ShouldDestroyWidget())
			{
				bShouldRemoveMessageWidgets = false;
				break;
			}
		}
	}
	
	if (bShouldRemoveMessageWidgets)
	{
		MessageWidgetContainer->ClearChildren();
	}
}
