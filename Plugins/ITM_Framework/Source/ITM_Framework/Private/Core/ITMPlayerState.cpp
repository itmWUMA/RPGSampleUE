// Copyright 2024 Itmwuma


#include "Core/ITMPlayerState.h"

#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AITMPlayerState::AITMPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UITMAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AITMPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AITMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AITMPlayerState, Level);
}

void AITMPlayerState::OnRep_Level(int32 OldValue)
{
	
}
