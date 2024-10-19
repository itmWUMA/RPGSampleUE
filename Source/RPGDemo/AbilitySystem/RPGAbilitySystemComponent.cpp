// Copyright 2024 Itmwuma


#include "AbilitySystem/RPGAbilitySystemComponent.h"

#include "RPGAbilitySystemDefines.h"
#include "RPGAttributeSetBase.h"
#include "Character/ITMCharacterBase.h"
#include "Combat/AbilitySystem/ITMAbilitySystemUtilities.h"
#include "Core/RPGPlayerState.h"
#include "Level/Load/RPGSaveGame.h"

FGameplayEffectContextHandle URPGAbilitySystemComponent::MakeDamageEffectContext(const FAttackData& InAttackData) const
{
	FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(
		FITMAbilitySystemUtilities::AllocEffectContext<FRPGDamageGameplayEffectContext>());
	
	// By default use the owner and avatar as the instigator and causer
	if (ensureMsgf(AbilityActorInfo.IsValid(), TEXT("Unable to make effect context because AbilityActorInfo is not valid.")))
	{
		Context.AddInstigator(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get());
	}

	static_cast<FRPGDamageGameplayEffectContext*>(Context.Get())->AttackData = InAttackData;
	
	return Context;
}

void URPGAbilitySystemComponent::UpgradeAttribute(const FGameplayAttribute& InAttribute)
{
	if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(GetOwnerActor()))
	{
		int32 CurPoint = PS->GetAttributePoint();
		if (CurPoint <= 0)
		{
			UE_LOG(LogAbilitySystemComponent, Warning,
				TEXT("Cannot upgrade attribute[%s] because current attribute point is empty."), *InAttribute.GetName());
			return;
		}
		
		const FString TagName = TEXT("Type.Attribute.") + InAttribute.GetName();
		FGameplayTag DataTag = FGameplayTag::RequestGameplayTag(FName(TagName));
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(UpgradeAttributeEffectClass, 1.f, MakeEffectContext());
		for (const FGameplayTag& Tag : UpgradeAttributeTags)
		{
			if (Tag.MatchesTagExact(DataTag))
			{
				SpecHandle.Data->SetSetByCallerMagnitude(Tag, 1.f);
			}
			else
			{
				SpecHandle.Data->SetSetByCallerMagnitude(Tag, 0.f);
			}
		}
		ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
		PS->AddToAttributePoint(-1);
	}
}

void URPGAbilitySystemComponent::InitAttributesFromSaveGame(URPGSaveGame* InSaveGame)
{
	if (!IsValid(InSaveGame))
		return;
	
	auto ApplyAttributeEffect =
		[WeakThis = TWeakObjectPtr<UITMAbilitySystemComponent>(this)](const TSubclassOf<UGameplayEffect>& EffectClass, float Level)
		{
			if (!WeakThis.IsValid())
				return;
			FGameplayEffectContextHandle ContextHandle = WeakThis->MakeEffectContext();
			ContextHandle.AddSourceObject(WeakThis->GetOwner());
			const FGameplayEffectSpecHandle SpecHandle =
				WeakThis->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
			if (SpecHandle.IsValid())
			{
				WeakThis->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		};

	auto SetSaveDataCallerMagnitude =
		[](const FGameplayEffectSpecHandle& Handle, const FString& AttributeName, float Magnitude)
		{
			FGameplayTag DataTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Type.Attribute.") + AttributeName));
			Handle.Data->SetSetByCallerMagnitude(DataTag, Magnitude);
		};

	const AITMCharacterBase* Avatar = Cast<AITMCharacterBase>(GetAvatarActor());
	const int32 Level = Avatar ? Avatar->GetCharacterLevel() : 1.f;

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(InitPrimaryAttributesFromSaveDataEffect, 1, MakeEffectContext());
	SetSaveDataCallerMagnitude(SpecHandle, URPGAttributeSetBase::GetStrengthAttribute().AttributeName, InSaveGame->Strength);
	SetSaveDataCallerMagnitude(SpecHandle, URPGAttributeSetBase::GetIntelligenceAttribute().AttributeName, InSaveGame->Intelligence);
	SetSaveDataCallerMagnitude(SpecHandle, URPGAttributeSetBase::GetResilienceAttribute().AttributeName, InSaveGame->Resilience);
	SetSaveDataCallerMagnitude(SpecHandle, URPGAttributeSetBase::GetVigorAttribute().AttributeName, InSaveGame->Vigor);
	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	ApplyAttributeEffect(AttributesCalculationEffectClass, Level);
	ApplyAttributeEffect(InitialVitalAttributesEffectClass, Level);
}
