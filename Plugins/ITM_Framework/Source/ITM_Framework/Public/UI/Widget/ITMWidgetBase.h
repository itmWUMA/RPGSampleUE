// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ITMWidgetBase.generated.h"

class UITMViewModelBase;

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|ViewModel")
	void SetViewModel(UITMViewModelBase* InViewModel);
	
	void SetWidgetInputMode(bool bShouldShowMouseCursor);
	FORCEINLINE TObjectPtr<UITMViewModelBase> GetViewModel() const { return ViewModel; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "UI|ViewModel")
	void OnViewModelInitialized();

	virtual void OnViewModelInitialized_Implementation() { }

	virtual void NativePreConstruct() override;

	// 初始化控件的样式，在PreConstruct时调用
	virtual void InitWidgetsStyle() { }
 
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UITMViewModelBase> ViewModel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic Widget Settings")
	bool bShowMouseCursor = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic Widget Settings")
	bool bDisableGameplayInput = false;
};
