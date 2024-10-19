// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "HurtTextWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class RPGDEMO_API UHurtTextWidget : public UITMWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitHurtTextWidget(float InDamageValue, const FVector& InSourceActorLocationCache, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void InitWidgetsStyle() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	void UpdateWidgetPos();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_DamageText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_BlockDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_CriticalDamage;

private:
	FVector SourceActorLocationCache = FVector::ZeroVector;
};
