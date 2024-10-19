// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/ITMWidgetBase.h"
#include "MainMenuWidget.generated.h"

class ULoadSystemData;
/**
 * 
 */
UCLASS()
class RPGDEMO_API UMainMenuWidget : public UITMWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void PushWidget(UWidget* InWidget);

	UFUNCTION(BlueprintCallable)
	void PopWidget();
	
	UFUNCTION(BlueprintCallable)
	void EnterGame();

	UFUNCTION(BlueprintCallable)
	void QuitGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TObjectPtr<USoundBase> MainMenuMusic;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<UWidget*> MainMenuWidgetStack;
};
