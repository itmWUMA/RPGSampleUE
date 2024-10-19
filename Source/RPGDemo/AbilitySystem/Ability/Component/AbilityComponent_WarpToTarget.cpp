// Copyright 2024 Itmwuma


#include "AbilitySystem/Ability/Component/AbilityComponent_WarpToTarget.h"

#include "MotionWarpingComponent.h"
#include "Character/RPGEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAbilityComponent_WarpToTarget::OnBegin(const FGameplayAbilitySpecHandle& Handle,
                                             const FGameplayAbilityActorInfo& ActorInfo)
{
	const ARPGEnemyCharacter* EnemyCharacter = Cast<ARPGEnemyCharacter>(ActorInfo.AvatarActor);
	if (!EnemyCharacter)
		return;
	
	if (UMotionWarpingComponent* MotionWarpingComp = EnemyCharacter->GetComponentByClass<UMotionWarpingComponent>())
	{
		if (const AActor* Target = EnemyCharacter->GetCombatTarget())
		{
			MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(WarpTargetName, Target->GetActorLocation());
		}
	}
}

void UAbilityComponent_WarpToPlayer::OnBegin(const FGameplayAbilitySpecHandle& Handle,
	const FGameplayAbilityActorInfo& ActorInfo)
{
	const ARPGEnemyCharacter* EnemyCharacter = Cast<ARPGEnemyCharacter>(ActorInfo.AvatarActor);
	if (!EnemyCharacter)
		return;
	
	if (UMotionWarpingComponent* MotionWarpingComp = EnemyCharacter->GetComponentByClass<UMotionWarpingComponent>())
	{
		if (const AActor* Target = UGameplayStatics::GetPlayerCharacter(EnemyCharacter, 0))
		{
			MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(WarpTargetName, Target->GetActorLocation());
		}
	}
}
