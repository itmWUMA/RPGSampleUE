// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ITMViewModelManager.h"
#include "ViewModel/ITMViewModelBase.h"

class UITMWidgetBase;

/**
 * 
 */
class ITM_FRAMEWORK_API FITMUserInterfaceUtilities
{
public:
	/**
	 * 根据控件类创建根控件，使用默认ViewModel
	 */
	template<typename WidgetType, typename ViewModelType>
	static WidgetType* CreateRootWidget(
		const UObject* WorldContext,
		TSubclassOf<WidgetType> WidgetClass,
		FName ViewModelId,
		const FITMViewModelParams& Params,
		TSubclassOf<ViewModelType> ViewModelClass = nullptr);

	static void HideWidget(UITMWidgetBase* Widget, bool bShouldShowMouseCursor = false);

	static void ResumeWidget(UITMWidgetBase* Widget);

	/**
	 * 创建默认的ViewModel参数
	 */
	static FITMViewModelParams MakeDefaultViewModelParams(const UObject* WorldContext);
};

template <typename WidgetType, typename ViewModelType>
WidgetType* FITMUserInterfaceUtilities::CreateRootWidget(const UObject* WorldContext,
	TSubclassOf<WidgetType> WidgetClass, FName ViewModelId, const FITMViewModelParams& Params,
	TSubclassOf<ViewModelType> ViewModelClass)
{
	static_assert(TIsDerivedFrom<WidgetType, UITMWidgetBase>::IsDerived
		|| TIsDerivedFrom<ViewModelType, UITMViewModelBase>::IsDerived
		, "WidgetType can only be derived from UITMWidgetBase. ViewModelType can only be derived from UITMViewModelBase");

	if (!WorldContext)
		return nullptr;
	
	WidgetType* Widget = CreateWidget<WidgetType>(WorldContext->GetWorld(), WidgetClass);
	if (Widget)
	{
		ViewModelType* ViewModel = nullptr;
		if (UITMViewModelManager* ViewModelMgr = UITMViewModelManager::Get(WorldContext))
		{
			if (ViewModelClass)
			{
				ViewModel = Cast<ViewModelType>(
					ViewModelMgr->GetOrCreateViewModel(ViewModelId, ViewModelClass, Params));
			}
			else
			{
				ViewModel = ViewModelMgr->GetOrCreateViewModel<ViewModelType>(ViewModelId, Params);
			}
		}

		Widget->AddToViewport();
		Widget->SetWidgetInputMode(true);

		if (ViewModel)
		{
			Widget->SetViewModel(ViewModel);
			ViewModel->OnInit();
		}
	}

	return Widget;
}