// Copyright 2024 Itmwuma


#include "Level/Load/LoadSystemData.h"

TSoftObjectPtr<UWorld> ULoadSystemData::GetMapByName(const FString& InString) const
{
	if (const TSoftObjectPtr<UWorld>* WorldPtr = Maps.Find(InString))
		return *WorldPtr;
	return nullptr;
}
