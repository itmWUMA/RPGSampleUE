// Copyright 2024 Itmwuma


#include "AbilitySystem/RPGAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "RPGAbilitySystemComponent.h"
#include "RPGAbilitySystemDefines.h"
#include "Character/RPGEnemyCharacter.h"
#include "Combat/AbilitySystem/Component/ITMHurtComponent.h"
#include "Core/FRPGGlobalDelegates.h"
#include "Core/RPGPlayerState.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "XP/AwardComponent.h"

URPGAttributeSetBase::URPGAttributeSetBase()
{
}

URPGAttributeSetBase::~URPGAttributeSetBase()
{
}

void URPGAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, ManaRegeneration, COND_None, REPNOTIFY_Always);
}

void URPGAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	PRE_ATTRIBUTE_CLAMP(Health, Attribute, NewValue, 0.f);
	PRE_ATTRIBUTE_CLAMP(Mana, Attribute, NewValue, 0.f);
}

void URPGAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectData EffectData;
	SetEffectData(Data, EffectData);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float LocalHealthValue = FMath::Clamp(GetHealth() - LocalIncomingDamage, 0.f, GetMaxHealth());
			SetHealth(LocalHealthValue);

			const bool bFatal = LocalHealthValue <= 0.f;
			const URPGAbilitySystemComponent* TargetASC = Cast<URPGAbilitySystemComponent>(EffectData.TargetData.ASC);
			if (!TargetASC)
				return;

			const FRPGDamageGameplayEffectContext* DamageContext = static_cast<FRPGDamageGameplayEffectContext*>(EffectData.ContextHandle.Get());
			if (!DamageContext)
				return;

			FRPGGlobalDelegates::OnCharacterHurtDelegate.Broadcast(
				EffectData.TargetData.AvatarActor,
				LocalIncomingDamage,
				DamageContext->bIsBlockedHit,
				DamageContext->bIsCriticalHit);
				
			if (!bFatal)
			{
				if (UITMHurtComponent* HurtComponent = Data.Target.GetAvatarActor()->GetComponentByClass<UITMHurtComponent>())
				{
					FHurtDataCollection DataCollection;
					DataCollection.HurtTypeTag = DamageContext->AttackData.PendingHurtType;
					HurtComponent->SetCurrentHurtData(MoveTemp(DataCollection));
				}
				Data.Target.TryActivateAbilitiesByTag(TargetASC->GetHurtAbilityTag().GetSingleTagContainer());
			}
			else
			{
				Data.Target.TryActivateAbilitiesByTag(TargetASC->GetDieAbilityTag().GetSingleTagContainer());
				// 给予经验奖励
				if (ARPGEnemyCharacter* EnemyCharacter = Cast<ARPGEnemyCharacter>(EffectData.TargetData.AvatarActor))
				{
					if (EffectData.SourceData.AvatarActor == UGameplayStatics::GetPlayerCharacter(EnemyCharacter, 0))
					{
						EnemyCharacter->GetAwardComponent()->GiveReward(EffectData.SourceData.AvatarActor, EnemyCharacter->GetCharacterLevel());
					}
				}
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);
		if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(Data.Target.GetOwnerActor()))
		{
			// 等级升级
			PS->AddToXP(LocalIncomingXP);
			int32 OldLevel = PS->GetPlayerLevel();
			int32 NewLevel = PS->GetLevelUpInfo()->GetLevelForXP(PS->GetXP()) + 1;
			PS->SetPlayerLevel(NewLevel);
			if (NewLevel != OldLevel)
			{
				PS->GetOnPlayerLevelChangedDelegate().Broadcast(OldLevel, NewLevel);
				// 生命、法力奖励
				if (NewLevel > OldLevel)
				{
                    SetHealth(GetMaxHealth());
                    SetMana(GetMaxMana());
				}
			}

			// 点数奖励
			NewLevel = FMath::Max(NewLevel, OldLevel);
			int32 TotalAttributeReward = 0;
			for (int32 i = OldLevel - 1; i < NewLevel - 1; ++i)
			{
				TotalAttributeReward += PS->GetLevelUpInfo()->GetAttributePointReward(i);
			}
			if (TotalAttributeReward > 0)
			{
				PS->AddToAttributePoint(TotalAttributeReward);
			}
		}
	}

	POST_ATTRIBUTE_CLAMP(Health, Data, 0.f);
	POST_ATTRIBUTE_CLAMP(Mana, Data, 0.f);
}

void URPGAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		SetHealth(GetMaxHealth());
	}

	if (Attribute == GetMaxManaAttribute())
	{
		SetMana(GetMaxMana());
	}
}

void URPGAttributeSetBase::SetEffectData(const FGameplayEffectModCallbackData& ModCallbackData,
                                         FGameplayEffectData& OutEffectData)
{
	const FGameplayEffectContextHandle ContextHandle = ModCallbackData.EffectSpec.GetContext();
	OutEffectData.ContextHandle = ContextHandle;

	UAbilitySystemComponent* SourceASC = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	SetEffectActorData(SourceASC, OutEffectData.SourceData);

	UAbilitySystemComponent* TargetASC = &ModCallbackData.Target;
	SetEffectActorData(TargetASC, OutEffectData.TargetData);
}

void URPGAttributeSetBase::SetEffectActorData(UAbilitySystemComponent* ASC,
	FGameplayEffectActorData& OutEffectActorData)
{
	if (ASC && ASC->AbilityActorInfo.IsValid())
	{
		OutEffectActorData.ASC = ASC;
		const TWeakObjectPtr<AActor> AvatarActor = ASC->AbilityActorInfo->AvatarActor;
		if (AvatarActor.IsValid())
		{
			OutEffectActorData.AvatarActor = AvatarActor.Get();
			if (ACharacter* Character = Cast<ACharacter>(AvatarActor.Get()))
			{
				OutEffectActorData.Character = Character;
				if (AController* Controller = Character->GetController())
				{
					OutEffectActorData.Controller = Controller;
				}
			}
		}
	}
}

void URPGAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, Health, OldValue);
}

void URPGAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, MaxHealth, OldValue);
}

void URPGAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, Mana, OldValue);
}

void URPGAttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, MaxMana, OldValue);
}

void URPGAttributeSetBase::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, Strength, OldValue);
}

void URPGAttributeSetBase::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, Intelligence, OldValue);
}

void URPGAttributeSetBase::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, Resilience, OldValue);
}

void URPGAttributeSetBase::OnRep_Vigor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, Vigor, OldValue);
}

void URPGAttributeSetBase::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, Armor, OldValue);
}

void URPGAttributeSetBase::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, ArmorPenetration, OldValue);
}

void URPGAttributeSetBase::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, BlockChance, OldValue);
}

void URPGAttributeSetBase::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, CriticalHitChance, OldValue);
}

void URPGAttributeSetBase::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, CriticalHitDamage, OldValue);
}

void URPGAttributeSetBase::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, CriticalHitResistance, OldValue);
}

void URPGAttributeSetBase::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, HealthRegeneration, OldValue);
}

void URPGAttributeSetBase::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, ManaRegeneration, OldValue);
}