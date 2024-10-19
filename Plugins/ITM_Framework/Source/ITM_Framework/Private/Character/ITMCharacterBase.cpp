// Copyright 2024 Itmwuma


#include "Character/ITMCharacterBase.h"

#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Settings/ITMFrameworkSettings.h"

AITMCharacterBase::AITMCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* AITMCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponentCache.Get();
}

int32 AITMCharacterBase::GetCharacterLevel() const
{
	return 0;
}

void AITMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

