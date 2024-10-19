// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ViewModel/ITMViewModelBase.h"
#include "ITMViewModelManager.generated.h"

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMViewModelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UITMViewModelManager* Get(const UObject* WorldContext);
	
	template<typename T>
	T* CreateViewModel(const FName& ViewModelId, const FITMViewModelParams& Params);
	template<typename T>
	T* GetOrCreateViewModel(const FName& ViewModelId, const FITMViewModelParams& Params);
	UITMViewModelBase* CreateViewModel(const FName& ViewModelId, const TSubclassOf<UITMViewModelBase> ViewModelClass, const FITMViewModelParams& Params);
	UITMViewModelBase* GetOrCreateViewModel(const FName& ViewModelId, const TSubclassOf<UITMViewModelBase> ViewModelClass, const FITMViewModelParams& Params);
	
	UITMViewModelBase* GetViewModel(const FName& ViewModelId) const;

private:
	UPROPERTY(Transient)
	TMap<FName, UITMViewModelBase*> ViewModels;
};

template <typename T>
T* UITMViewModelManager::CreateViewModel(const FName& ViewModelId, const FITMViewModelParams& Params)
{
	// 检查ViewModel类型合法性
	UClass* ViewModelClass = T::StaticClass();
	checkf(ViewModelClass, TEXT("Create view model called with a nullptr class object"));
	checkf(ViewModelClass->IsChildOf(UITMViewModelBase::StaticClass()), TEXT("Create view model called with invalid class, %s must be a child of %s"), *ViewModelClass->GetName(), *ViewModelClass->GetName());

	// 无缓存ViewModel，则新建ViewModel
	T* ViewModel = NewObject<T>(this, ViewModelClass);
	ViewModel->SetViewModelParams(Params);
	ViewModels.Add(ViewModelId, ViewModel);

	return ViewModel;
}

template <typename T>
T* UITMViewModelManager::GetOrCreateViewModel(const FName& ViewModelId, const FITMViewModelParams& Params)
{
	// 检查ViewModel类型合法性
	UClass* ViewModelClass = T::StaticClass();
	checkf(ViewModelClass, TEXT("Create view model called with a nullptr class object"));
	checkf(ViewModelClass->IsChildOf(UITMViewModelBase::StaticClass()), TEXT("Create view model called with invalid class, %s must be a child of %s"), *ViewModelClass->GetName(), *ViewModelClass->GetName());
	
	if (UITMViewModelBase** ViewModelPtr = ViewModels.Find(ViewModelId))
	{
		if (*ViewModelPtr)
		{
			return Cast<T>(*ViewModelPtr);
		}
	}

	return CreateViewModel<T>(ViewModelId, Params);
}
