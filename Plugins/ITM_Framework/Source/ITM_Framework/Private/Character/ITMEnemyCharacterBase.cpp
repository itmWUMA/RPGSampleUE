// Copyright 2024 Itmwuma


#include "Character/ITMEnemyCharacterBase.h"

#include "AI/ITMAIController.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"

AITMEnemyCharacterBase::AITMEnemyCharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UITMAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AbilitySystemComponentCache = AbilitySystemComponent;
}

int32 AITMEnemyCharacterBase::GetCharacterLevel() const
{
	return Level;
}

void AITMEnemyCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
		return;

	AIControllerCache = Cast<AITMAIController>(NewController);
	if (!AIControllerCache.IsValid())
		return;

	AIControllerCache->InitAI(BTAsset);
}

void AITMEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
		return;

	InitAbilityActorInfo();

	AbilitySystemComponent->GrantAbilities();
}

void AITMEnemyCharacterBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AbilityActorInfoSet();
	AbilitySystemComponent->InitAttributes();
}
