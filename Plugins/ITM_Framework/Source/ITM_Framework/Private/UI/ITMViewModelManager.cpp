// Copyright 2024 Itmwuma


#include "UI/ITMViewModelManager.h"

#include "Kismet/GameplayStatics.h"

UITMViewModelManager* UITMViewModelManager::Get(const UObject* WorldContext)
{
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContext))
	{
		return GameInstance->GetSubsystem<UITMViewModelManager>();
	}
	return nullptr;
}

UITMViewModelBase* UITMViewModelManager::CreateViewModel(const FName& ViewModelId,
	const TSubclassOf<UITMViewModelBase> ViewModelClass, const FITMViewModelParams& Params)
{
	if (!ViewModelClass)
		return nullptr;

	// 无缓存ViewModel，则新建ViewModel
	UITMViewModelBase* ViewModel = NewObject<UITMViewModelBase>(this, ViewModelClass);
	ViewModel->SetViewModelParams(Params);
	ViewModels.Add(ViewModelId, ViewModel);

	return ViewModel;
}

UITMViewModelBase* UITMViewModelManager::GetOrCreateViewModel(const FName& ViewModelId,
                                                              const TSubclassOf<UITMViewModelBase> ViewModelClass, const FITMViewModelParams& Params)
{
	if (UITMViewModelBase** ViewModelPtr = ViewModels.Find(ViewModelId))
	{
		if (*ViewModelPtr)
		{
			return Cast<UITMViewModelBase>(*ViewModelPtr);
		}
	}

	return CreateViewModel(ViewModelId, ViewModelClass, Params);
}

UITMViewModelBase* UITMViewModelManager::GetViewModel(const FName& ViewModelId) const
{
	if (const auto ViewModelPtr = ViewModels.Find(ViewModelId))
	{
		return *ViewModelPtr;
	}
	return nullptr;
}
