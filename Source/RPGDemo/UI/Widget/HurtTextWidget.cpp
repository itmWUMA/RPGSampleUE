// Copyright 2024 Itmwuma


#include "UI/Widget/HurtTextWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/ITMViewModelBase.h"

void UHurtTextWidget::InitHurtTextWidget(float InDamageValue, const FVector& InSourceActorLocationCache, bool bBlockedHit, bool bCriticalHit)
{
	Text_DamageText->SetText(FText::AsNumber(InDamageValue));
	SourceActorLocationCache = InSourceActorLocationCache;

	if (bCriticalHit)
		PlayAnimationForward(Anim_CriticalDamage);
	else if (bBlockedHit)
		PlayAnimationForward(Anim_BlockDamage);
	else
		PlayAnimationForward(Anim_Damage);

	UpdateWidgetPos();
}

void UHurtTextWidget::InitWidgetsStyle()
{
	if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Text_DamageText->Slot))
	{
		OverlaySlot->SetHorizontalAlignment(HAlign_Center);
		OverlaySlot->SetVerticalAlignment(VAlign_Center);
	}
}

void UHurtTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHurtTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateWidgetPos();
}

void UHurtTextWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Anim_Damage == Animation ||
		Anim_CriticalDamage == Animation ||
		Anim_BlockDamage == Animation)
	{
		RemoveFromParent();
	}
}

void UHurtTextWidget::UpdateWidgetPos()
{
	if (!ViewModel)
		return;
	
	FVector2D ResultPos = FVector2D::ZeroVector;
	if (UGameplayStatics::ProjectWorldToScreen(
		ViewModel->GetViewModelParams().PlayerControllerCache,
		SourceActorLocationCache,
		ResultPos))
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			ResultPos /= UWidgetLayoutLibrary::GetViewportScale(this);
			CanvasSlot->SetPosition(ResultPos);
		}
	}
	else
	{
		RemoveFromParent();
	}
}
