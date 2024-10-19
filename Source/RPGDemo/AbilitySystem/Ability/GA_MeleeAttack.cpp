// Copyright 2024 Itmwuma


#include "AbilitySystem/Ability/GA_MeleeAttack.h"

#include "NiagaraFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/RPGAttackComponent.h"
#include "Character/CharacterDeath.h"
#include "Character/RPGEnemyCharacter.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

void UGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitDetectTag);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UGA_MeleeAttack::OnHitDetectTagReceived);
	WaitGameplayEventTask->Activate();
}

void UGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitGameplayEventTask)
	{
		WaitGameplayEventTask->EventReceived.RemoveAll(this);
		WaitGameplayEventTask = nullptr;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_MeleeAttack::OnHitDetectTagReceived(FGameplayEventData Payload)
{
	AttackTarget(Payload.Instigator, const_cast<AActor*>(Payload.Target.Get()));
}

bool UGA_MeleeAttack::CanAttack(const AActor* Instigator, const AActor* Target) const
{
	if (Instigator != GetAvatarActorFromActorInfo())
		return false;

	const ICharacterDeath* DeathInterface = Cast<ICharacterDeath>(Target);
	if (DeathInterface && DeathInterface->IsDead())
		return false;

	if (bOnlyAttackCombatTarget)
	{
		if (const ARPGEnemyCharacter* EnemyCharacter = Cast<ARPGEnemyCharacter>(Instigator))
		{
			return EnemyCharacter->GetCombatTarget() == Target;
		}
	}

	return true;
}

void UGA_MeleeAttack::AttackTarget(const AActor* Instigator, AActor* Target)
{
	if (!CanAttack(Instigator, Target))
		return;

	if (URPGAttackComponent* AttackComp = Instigator->GetComponentByClass<URPGAttackComponent>())
	{
		AttackComp->Init(Cast<UITMAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()), GetCurrentAbilitySpecHandle());
		AttackComp->Attack(Target, AttackData);

		// 播放命中效果
		const FHitResult& HitResult = AttackComp->GetCurrentHitResult();
		if (HitResult.HasValidHitObjectHandle())
		{
			FGameplayCueParameters Params;
			Params.Location = HitResult.Location;
			K2_ExecuteGameplayCueWithParams(ImpactCueTag, Params);
		}

		AttackComp->ClearHitResult();
	}
}
