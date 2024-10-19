// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Entity/ITMEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AITMEffectActor::AITMEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AITMEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AITMEffectActor::ApplyEffectToTarget(AActor* EffectTarget)
{
	if (!CanApplyEffect(EffectTarget))
		return false;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectTarget))
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GEClass, Level, ContextHandle);
		const FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (GEClass->GetDefaultObject<UGameplayEffect>()->DurationPolicy == EGameplayEffectDurationType::Infinite)
		{
			EffectHandles.Add(EffectTarget, EffectHandle);
		}

		return true;
	}

	return false;
}

void AITMEffectActor::RemoveEffect(AActor* EffectTarget)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectTarget))
	{
		if (const FActiveGameplayEffectHandle* EffectHandlePtr = EffectHandles.Find(EffectTarget))
		{
			ASC->RemoveActiveGameplayEffect(*EffectHandlePtr);
			EffectHandles.Remove(EffectTarget);
		}
	}
}

bool AITMEffectActor::CanApplyEffect_Implementation(AActor* PendingTarget)
{
	for (const FName& IgnoreTag : IgnoreTags)
	{
		if (PendingTarget->ActorHasTag(IgnoreTag))
			return false;
	}
	return true;
}
