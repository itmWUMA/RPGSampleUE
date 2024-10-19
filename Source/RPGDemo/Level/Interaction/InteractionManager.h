// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Highlightable.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractionManager.generated.h"

class IHighlightable;
/**
 * 
 */
UCLASS()
class RPGDEMO_API UInteractionManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual TStatId GetStatId() const override;
	
	void RegisterHighlightable(AActor* HighlightableObject);
	void UnregisterHighlightable(AActor* HighlightableObject);
	FORCEINLINE const TArray<AActor*>& GetHighlightableObjects() const { return HighlightableObjects; }

	void RegisterHighlighted(AActor* HighlightedObject);
	void UnregisterHighlighted(AActor* HighlightedObject);

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetClosestHighlightableObject(const AActor* SourceActor) const;
	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetClosestHighlightedObject(const AActor* SourceActor) const;

private:
	static AActor* GetClosestObject_Internal(const AActor* SourceActor, const TArray<AActor*>& CandidateObjects);

private:
	UPROPERTY(Transient)
	TArray<AActor*> HighlightableObjects;
	UPROPERTY(Transient)
	TArray<AActor*> HighlightedObjects;
};
