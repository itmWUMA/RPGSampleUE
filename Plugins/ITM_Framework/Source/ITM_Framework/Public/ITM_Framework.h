// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FITM_FrameworkModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
