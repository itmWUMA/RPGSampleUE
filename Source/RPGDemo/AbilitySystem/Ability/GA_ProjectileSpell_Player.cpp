// Copyright 2024 Itmwuma


#include "AbilitySystem/Ability/GA_ProjectileSpell_Player.h"

#include "AbilitySystem/Projectile/ProjectileAimPolicy.h"
#include "Core/RPGPlayerController.h"

FTransform UGA_ProjectileSpell_Player::GetSpawnTransform(const FGameplayAbilityActorInfo& ActorInfo) const
{
	FTransform BaseTransform = Super::GetSpawnTransform(ActorInfo);
	if (ARPGPlayerController* PC = Cast<ARPGPlayerController>(ActorInfo.PlayerController))
	{
		if (PC->IsSkillModifyTriggered() && SkillModifyAimPolicy)
		{
			BaseTransform.SetRotation(SkillModifyAimPolicy->GetAimRotation(
				ActorInfo.AvatarActor.Get(), BaseTransform).Quaternion());
		}
	}
	
	return BaseTransform;
}

FRotator UGA_ProjectileSpell_Player::GetFacingTargetRotation(const FGameplayAbilityActorInfo& ActorInfo) const
{
	if (ARPGPlayerController* PC = Cast<ARPGPlayerController>(ActorInfo.PlayerController))
	{
		if (PC->IsSkillModifyTriggered() && SkillModifyAimPolicy)
		{
			return SkillModifyAimPolicy->GetAimRotation(ActorInfo.AvatarActor.Get(),
				ActorInfo.AvatarActor->GetActorTransform());
		}
	}
	return Super::GetFacingTargetRotation(ActorInfo);
}
