// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveActor.generated.h"

UINTERFACE(MinimalAPI)
class USaveActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGDEMO_API ISaveActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool ShouldLoadTransform();
	virtual bool ShouldLoadTransform_Implementation() { return false; }
	
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();
	virtual void OnActorLoaded_Implementation() { }
};
