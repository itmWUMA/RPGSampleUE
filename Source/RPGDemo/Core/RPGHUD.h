// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/ViewModel/ITMViewModelBase.h"
#include "RPGHUD.generated.h"

class URootHUDViewModel;
class URootHUD;

/**
 * 
 */
UCLASS()
class RPGDEMO_API ARPGHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitHUD(const FITMViewModelParams& Params);

protected:
	UPROPERTY(Transient)
	TObjectPtr<URootHUD> HUDWidget;
};
