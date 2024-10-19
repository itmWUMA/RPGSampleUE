// Copyright 2024 Itmwuma


#include "UI/ITMUIBlueprintFunctionLibrary.h"

#include "UI/ITMUserInterfaceUtilities.h"
#include "UI/ITMViewModelManager.h"

UITMViewModelBase* UITMUIBlueprintFunctionLibrary::GetViewModel(const UObject* WorldContext, const FName& ViewModelId)
{
	if (const UITMViewModelManager* ViewModelMgr = UITMViewModelManager::Get(WorldContext))
	{
		return ViewModelMgr->GetViewModel(ViewModelId);
	}
	return nullptr;
}

FITMViewModelParams UITMUIBlueprintFunctionLibrary::MakeDefaultViewModelParams(const UObject* WorldContext)
{
	return FITMUserInterfaceUtilities::MakeDefaultViewModelParams(WorldContext);
}
