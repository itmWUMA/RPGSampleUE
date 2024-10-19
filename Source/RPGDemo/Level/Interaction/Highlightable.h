// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Highlightable.generated.h"

UINTERFACE(MinimalAPI)
class UHighlightable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGDEMO_API IHighlightable
{
	GENERATED_BODY()

public:
	virtual void RegisterHighlightActor(const UObject* WorldContext);
	virtual void UnregisterHighlightActor(const UObject* WorldContext);
	virtual void HighlightActor() = 0;
	virtual void UnhighlightActor() = 0;
};
