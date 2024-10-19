// Copyright 2024 Itmwuma


#include "Level/Interaction/InteractionManager.h"

TStatId UInteractionManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UInteractionManager, STATGROUP_Tickables);
}

void UInteractionManager::RegisterHighlightable(AActor* HighlightableObject)
{
	if (!HighlightableObject ||
		HighlightableObjects.Contains(HighlightableObject) ||
		!HighlightableObject->GetClass()->ImplementsInterface(UHighlightable::StaticClass()))
	{
		return;
	}

	HighlightableObjects.Add(HighlightableObject);
}

void UInteractionManager::UnregisterHighlightable(AActor* HighlightableObject)
{
	UnregisterHighlighted(HighlightableObject);
	
	if (!HighlightableObjects.Contains(HighlightableObject))
	{
		return;
	}

	HighlightableObjects.RemoveSingleSwap(HighlightableObject);
}

void UInteractionManager::RegisterHighlighted(AActor* HighlightedObject)
{
	if (!HighlightedObjects.Contains(HighlightedObject))
	{
		if (IHighlightable* Highlightable =Cast<IHighlightable>(HighlightedObject))
		{
			Highlightable->HighlightActor();
			HighlightedObjects.Add(HighlightedObject);
		}
	}
}

void UInteractionManager::UnregisterHighlighted(AActor* HighlightedObject)
{
	if (HighlightedObjects.Contains(HighlightedObject))
	{
		if (IHighlightable* Highlightable =Cast<IHighlightable>(HighlightedObject))
		{
			Highlightable->UnhighlightActor();
			HighlightedObjects.RemoveSingleSwap(HighlightedObject);
		}
	}
}

AActor* UInteractionManager::GetClosestHighlightableObject(const AActor* SourceActor) const
{
	return GetClosestObject_Internal(SourceActor, HighlightableObjects);
}

AActor* UInteractionManager::GetClosestHighlightedObject(const AActor* SourceActor) const
{
	return GetClosestObject_Internal(SourceActor, HighlightedObjects);
}

AActor* UInteractionManager::GetClosestObject_Internal(const AActor* SourceActor,
	const TArray<AActor*>& CandidateObjects)
{
	if (CandidateObjects.IsEmpty() || !SourceActor)
		return nullptr;

	auto GetActorDis = [](const AActor* A, const AActor* B) -> float
	{
		return FVector::DistSquared(A->GetActorLocation(), B->GetActorLocation());
	};

	AActor* TargetActor = CandidateObjects[0];
	float TargetDis = GetActorDis(SourceActor, TargetActor);
	for (int32 i = 1; i < CandidateObjects.Num(); ++i)
	{
		const float CurDis = GetActorDis(SourceActor, CandidateObjects[i]);
		if (TargetDis > CurDis)
		{
			TargetDis = CurDis;
			TargetActor = CandidateObjects[i];
		}
	}

	return TargetActor;
}
