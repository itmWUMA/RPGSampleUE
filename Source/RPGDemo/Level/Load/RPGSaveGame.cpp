// Copyright 2024 Itmwuma


#include "Level/Load/RPGSaveGame.h"

bool FSavedActor::operator==(const FSavedActor& InSavedActor) const
{
	return this->ActorName == InSavedActor.ActorName;
}

FSavedMap URPGSaveGame::GetSavedMapWithMapName(const FString& InMapName) const
{
	for (const FSavedMap& Map :SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
			return Map;
	}
	return FSavedMap();
}

bool URPGSaveGame::HasMap(const FString& InMapName) const
{
	return SavedMaps.ContainsByPredicate([&InMapName](const FSavedMap& InMap) -> bool
		{
			return InMap.MapAssetName == InMapName;
		});
}
