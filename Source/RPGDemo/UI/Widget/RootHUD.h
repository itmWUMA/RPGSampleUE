// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/RootHUDViewModel.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "RootHUD.generated.h"

class UXpBar;
class UHurtTextWidget;
class UVerticalBox;
class UMessageWidget;
class UGlobeHealthBar;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class RPGDEMO_API URootHUD : public UITMWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void InitWidgetsStyle() override;
	virtual void OnViewModelInitialized_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "UI|RootHUD")
	UMessageWidget* CreateMessageWidget(const FTagUIWidgetRow& Data);

	UFUNCTION(BlueprintCallable, Category = "UI|RootHUD")
	UHurtTextWidget* CreateHurtTextWidget(const FVector& SourcePos, float DamageValue, bool bBlockedHit, bool bCriticalHit);

private:
	void InitGlobeBar(UGlobeHealthBar* InGlobeBar);
	void CheckAndRemoveMessageWidgets();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> Canvas_Root;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGlobeHealthBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGlobeHealthBar> ManaBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UXpBar> XpBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> MessageWidgetContainer;

	UPROPERTY(EditAnywhere, Category = "RootHUD|MessageWidget")
	TSubclassOf<UMessageWidget> DefaultMessageWidgetClass;

	UPROPERTY(EditAnywhere, Category = "RootHUD|HurtTextWidget")
	TSubclassOf<UHurtTextWidget> HurtTextWidgetClass;
};
