// Copyright 2024 Itmwuma


#include "AbilitySystem/SummonHandleComponent.h"

#include "Character/RPGEnemyCharacter.h"


void USummonHandleComponent::RegisterSummonedActor(const FGameplayTag& SummonAbilityTag, AActor* SummonedActor)
{
	SummonedCacheMap.FindOrAdd(SummonAbilityTag).AddUnique(SummonedActor);
	if (ARPGEnemyCharacter* EnemyCharacter = Cast<ARPGEnemyCharacter>(SummonedActor))
	{
		EnemyCharacter->GetOnEnemyDeadDelegate().AddWeakLambda(
			this, [this, AbilityTag = SummonAbilityTag, WeakSummonActor = MakeWeakObjectPtr<AActor>(SummonedActor)]()
		{
			UnregisterSummonedActor(AbilityTag, WeakSummonActor.Get());
		});
	}
}

void USummonHandleComponent::UnregisterSummonedActor(const FGameplayTag& SummonAbilityTag, AActor* SummonedActor)
{
	if (TArray<TWeakObjectPtr<AActor>>* CacheArrayPtr = SummonedCacheMap.Find(SummonAbilityTag))
	{
		CacheArrayPtr->RemoveSingleSwap(SummonedActor);
	}
}

bool USummonHandleComponent::GetRegisteredSummonActorByAbilityTag(const FGameplayTag& SummonAbilityTag, TArray<TWeakObjectPtr<AActor>>& OutCachedArray)
{
	TArray<TWeakObjectPtr<AActor>>* CacheArrayPtr = SummonedCacheMap.Find(SummonAbilityTag);
	if (!CacheArrayPtr)
		return false;
	OutCachedArray.Empty();
	for (const TWeakObjectPtr<AActor>& CachedActor : *CacheArrayPtr)
	{
		if (CachedActor.IsValid())
		{
			OutCachedArray.Add(CachedActor);
		}
	}
	*CacheArrayPtr = OutCachedArray;
	return true;
}
