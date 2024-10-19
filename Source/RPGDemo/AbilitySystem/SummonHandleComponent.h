// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SummonHandleComponent.generated.h"


UCLASS( ClassGroup=(Custom), DisplayName = "Summon Handler", meta=(BlueprintSpawnableComponent) )
class RPGDEMO_API USummonHandleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void RegisterSummonedActor(const FGameplayTag& SummonAbilityTag, AActor* SummonedActor);
	void UnregisterSummonedActor(const FGameplayTag& SummonAbilityTag, AActor* SummonedActor);
	bool GetRegisteredSummonActorByAbilityTag(const FGameplayTag& SummonAbilityTag, TArray<TWeakObjectPtr<AActor>>& OutCachedArray);

protected:
	TMap<FGameplayTag, TArray<TWeakObjectPtr<AActor>>> SummonedCacheMap;
};
