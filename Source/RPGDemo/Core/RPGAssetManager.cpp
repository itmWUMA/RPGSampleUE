// Copyright 2024 Itmwuma


#include "Core/RPGAssetManager.h"

#include "AbilitySystemGlobals.h"

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
