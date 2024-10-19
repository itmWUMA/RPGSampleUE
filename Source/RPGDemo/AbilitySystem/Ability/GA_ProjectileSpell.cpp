// Copyright 2024 Itmwuma


#include "AbilitySystem/Ability/GA_ProjectileSpell.h"

#include "AbilitySystem/Projectile/ProjectileAimPolicy.h"
#include "AbilitySystem/Projectile/RPGProjectile.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Combat/AbilitySystem/Component/ITMAttackComponent.h"
#include "Combat/Projectile/ITMLaunchObject.h"
#include "Combat/Weapon/ITMWeaponComponent.h"
#include "GameFramework/Character.h"

void UGA_ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

AITMLaunchObject* UGA_ProjectileSpell::SpawnProjectile()
{
	const FGameplayAbilityActorInfo& ActorInfo = GetActorInfo();
	const bool bIsServer = ActorInfo.AvatarActor->HasAuthority();
	if (!bIsServer)
		return nullptr;

	UWorld* World = GetWorld();
	if (!World)
		return nullptr;

	const FTransform& SpawnTransform = GetSpawnTransform(ActorInfo);
	ARPGProjectile* Projectile = World->SpawnActorDeferred<ARPGProjectile>(
		LaunchObjectClass,
		SpawnTransform,
		ActorInfo.AvatarActor.Get(),
		 Cast<APawn>(ActorInfo.AvatarActor.Get()),
		 ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->SetInstigator(Cast<APawn>(ActorInfo.AvatarActor.Get()));
	Projectile->GetAttackComponent()->Init(Cast<UITMAbilitySystemComponent>(ActorInfo.AbilitySystemComponent), GetCurrentAbilitySpecHandle());
	Projectile->SetAttackData(AttackData);

	Projectile->FinishSpawning(SpawnTransform);
	return Projectile;
}

FTransform UGA_ProjectileSpell::GetSpawnTransform(const FGameplayAbilityActorInfo& ActorInfo) const
{
	FTransform ResultTransform = FTransform::Identity;
	const ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo.AvatarActor);
	if (!AvatarCharacter)
		return ResultTransform;

	FTransform LocalTransform = FTransform::Identity;
	switch (SpawnPositionType) {
	case EProjectileSpawnPositionType::CharacterMesh:
		LocalTransform = AvatarCharacter->GetMesh()->GetComponentTransform();
		break;
	case EProjectileSpawnPositionType::CharacterSocket:
		LocalTransform = AvatarCharacter->GetMesh()->GetSocketTransform(SocketName);
		break;
	case EProjectileSpawnPositionType::WeaponSocket:
		if (const UITMWeaponComponent* WeaponComp = AvatarCharacter->GetComponentByClass<UITMWeaponComponent>())
		{
			LocalTransform = WeaponComp->GetWeaponMesh()->GetSocketTransform(SocketName);
		}
		break;
	default:
		checkNoEntry();
	}
	
	ResultTransform.SetLocation(LocalTransform.TransformPosition(Offset));

	if (AimPolicy)
	{
		ResultTransform.SetRotation(AimPolicy->GetAimRotation(AvatarCharacter, ResultTransform).Quaternion());
	}
	else
	{
		ResultTransform.SetRotation(LocalTransform.GetRotation());
	}
	
	return ResultTransform;
}

FRotator UGA_ProjectileSpell::GetFacingTargetRotation(const FGameplayAbilityActorInfo& ActorInfo) const
{
	if (!ActorInfo.AvatarActor.IsValid())
		return FRotator::ZeroRotator;
	
	if (!AimPolicy)
		return ActorInfo.AvatarActor->GetActorRotation();

	return AimPolicy->GetAimRotation(ActorInfo.AvatarActor.Get(), ActorInfo.AvatarActor->GetActorTransform());
}