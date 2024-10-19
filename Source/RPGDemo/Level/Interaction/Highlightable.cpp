// Copyright 2024 Itmwuma


#include "Level/Interaction/Highlightable.h"

#include "InteractionManager.h"

void IHighlightable::RegisterHighlightActor(const UObject* WorldContext)
{
	const UWorld* World = WorldContext->GetWorld();
	if (!World)
		return;

	AActor* ThisActor = Cast<AActor>(this);
	if (!ThisActor)
		return;

	if (UInteractionManager* InteractionMgr = World->GetSubsystem<UInteractionManager>())
	{
		InteractionMgr->RegisterHighlightable(ThisActor);
	}
}

void IHighlightable::UnregisterHighlightActor(const UObject* WorldContext)
{
	const UWorld* World = WorldContext->GetWorld();
	if (!World)
		return;

	AActor* ThisActor = Cast<AActor>(this);
	if (!ThisActor)
    	return;
	
	if (UInteractionManager* InteractionMgr = World->GetSubsystem<UInteractionManager>())
	{
		InteractionMgr->UnregisterHighlightable(ThisActor);
	}
}
