// Copyright 2024 Itmwuma


#include "UI/ViewModel/EnemyStateViewModel.h"

#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Kismet/GameplayStatics.h"

bool UEnemyStateViewModel::CanShowWidget() const
{
	if (!Params.ASCCache)
		return false;

	if (const AActor* TargetActor = Params.ASCCache->GetAvatarActor())
	{
		FVector2D ScreenPos = FVector2D::ZeroVector;
		const bool bValid = UGameplayStatics::ProjectWorldToScreen(
			UGameplayStatics::GetPlayerController(Params.ASCCache.Get(), 0),
			TargetActor->GetActorLocation(),
			ScreenPos);
		return bValid;
	}

	return false;
}

void UEnemyStateViewModel::BroadcastInitialValues()
{
	if (!Params.AttributeSetCache)
		return;

	if (const URPGAttributeSetBase* RPGAttributeSet = Cast<URPGAttributeSetBase>(Params.AttributeSetCache))
    {
		CurHealth = RPGAttributeSet->GetHealth();
		CurMaxHealth = RPGAttributeSet->GetMaxHealth();
    	OnHealthChanged.Broadcast(CurHealth);
    	OnMaxHealthChanged.Broadcast(CurMaxHealth);
    }
}

void UEnemyStateViewModel::BindCallbackToDependencies()
{
	if (!Params.ASCCache)
		return;

	if (const URPGAttributeSetBase* RPGAttributeSet = Cast<URPGAttributeSetBase>(Params.AttributeSetCache))
	{
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetHealthAttribute()).AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
			if (Data.NewValue < CurHealth)
				OnEnemyHealthReduce.Broadcast();
			CurHealth = Data.NewValue;
		});
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxHealthAttribute()).AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
			CurMaxHealth = Data.NewValue;
		});
	}
}

float UEnemyStateViewModel::GetDesiredWidgetScale() const
{
	if (!Params.ASCCache)
		return 1.f;

	const APlayerCameraManager* CamMgr = UGameplayStatics::GetPlayerCameraManager(Params.ASCCache.Get(), 0);
	if (!CamMgr)
		return 1.f;

	if (const AActor* TargetActor = Params.ASCCache->GetAvatarActor())
	{
		const FTransform TargetTransform = TargetActor->GetTransform();
		const FVector CameraRelativePos = TargetTransform.InverseTransformPosition(CamMgr->GetCameraLocation());
		const float Dis = CameraRelativePos.Size2D();
		if (WidgetScaleCurve)
		{
			return WidgetScaleCurve->GetFloatValue(Dis);
		}
	}

	return 1.f;
}
