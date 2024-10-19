// Copyright 2024 Itmwuma


#include "AbilitySystem/Ability/GA_Die.h"

#include "Character/CharacterDeath.h"
#include "Combat/Weapon/ITMWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UGA_Die::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return;

	// 解除角色武器
	if (UITMWeaponComponent* WeaponComponent = ActorInfo->AvatarActor->GetComponentByClass<UITMWeaponComponent>())
	{
		WeaponComponent->GetOnWeaponDetachedDelegate().AddWeakLambda(this, [](USkeletalMeshComponent* WeaponMesh)
		{
			WeaponMesh->SetSimulatePhysics(true);
			WeaponMesh->SetEnableGravity(true);
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		});
		WeaponComponent->DetachWeapon();
	}

	// 设置角色布娃娃化
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		Character->GetMesh()->SetSimulatePhysics(true);
		Character->GetMesh()->SetEnableGravity(true);
		Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Character->GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 播放死亡效果
	FGameplayCueParameters Params;
	Params.Location = ActorInfo->AvatarActor->GetActorLocation();
	K2_ExecuteGameplayCueWithParams(DeathCueTag, Params);

	if (ICharacterDeath* CharacterDeathInterface = Cast<ICharacterDeath>(CurrentActorInfo->AvatarActor))
	{
		CharacterDeathInterface->StartDie();
	}

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DestroyDelayHandle, this, &UGA_Die::OnStartDestroyCharacter,
			DestroyCharacterDelayTime, false);
	}
}

void UGA_Die::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DestroyDelayHandle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Die::OnStartDestroyCharacter()
{
	if (ICharacterDeath* CharacterDeathInterface = Cast<ICharacterDeath>(CurrentActorInfo->AvatarActor))
	{
		CharacterDeathInterface->OnDead();
	}
	else
	{
		CurrentActorInfo->AvatarActor->SetLifeSpan(DestroyCharacterDelayTime);
	}
			
	K2_EndAbility();
}
