// Copyright 2024 Itmwuma


#include "Character/ITMPlayerCharacterBase.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Core/ITMPlayerState.h"

void AITMPlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务端 初始化AbilityActorInfo
	InitAbilityActorInfo();

	// 赋予默认GA（GA仅在服务器上）
	if (AbilitySystemComponentCache.IsValid())
	{
		AbilitySystemComponentCache->GrantAbilities();
	}
}

void AITMPlayerCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 客户端 初始化AbilityActorInfo
	InitAbilityActorInfo();
}

int32 AITMPlayerCharacterBase::GetCharacterLevel() const
{
	if (const AITMPlayerState* PS = GetPlayerState<AITMPlayerState>())
		return PS->GetPlayerLevel();
	return Super::GetCharacterLevel();
}

void AITMPlayerCharacterBase::InitAbilityActorInfo()
{
	if (AITMPlayerState* PS = GetPlayerState<AITMPlayerState>())
	{
		if (UITMAbilitySystemComponent* ASC = Cast<UITMAbilitySystemComponent>(PS->GetAbilitySystemComponent()))
		{
			ASC->InitAbilityActorInfo(PS, this);
			ASC->InitAttributes();
			AbilitySystemComponentCache = ASC;
			AttributeSetCache = PS->GetAttributeSet();

			ASC->AbilityActorInfoSet();
			InitHUD();
		}
	}
}
