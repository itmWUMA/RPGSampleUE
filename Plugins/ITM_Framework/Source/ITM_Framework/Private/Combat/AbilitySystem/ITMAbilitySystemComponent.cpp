// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"

#include "Character/ITMCharacterBase.h"
#include "Combat/AbilitySystem/ITMAbilitySystemUtilities.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"
#include "Combat/AbilitySystem/ITMAbilityDefines.h"

void UITMAbilitySystemComponent::AbilityActorInfoSet()
{
	OnEffectAppliedHandle = OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UITMAbilitySystemComponent::ClientOnEffectApplied);
}

void UITMAbilitySystemComponent::ClientOnEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);
	AssetTagsAppliedDg.Broadcast(AssetTags);
}

void UITMAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UITMAbilitySystemComponent::InitAttributes()
{
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

	const AITMCharacterBase* Avatar = Cast<AITMCharacterBase>(GetAvatarActor());
	const int32 Level = Avatar ? Avatar->GetCharacterLevel() : 1.f;
	
	ApplyAttributeEffect(InitialPrimaryAttributesEffectClass, Level);
	ApplyAttributeEffect(AttributesCalculationEffectClass, Level);
	ApplyAttributeEffect(InitialVitalAttributesEffectClass, Level);
}

void UITMAbilitySystemComponent::GrantAbilities()
{
	for (const TSubclassOf<UITMGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		FGameplayAbilitySpec Spec(AbilityClass, 1);
		GiveAbility(Spec);
	}
}

bool UITMAbilitySystemComponent::TryActivateSingleAbilityByTag(const FGameplayTag& AbilityTag,
	bool bAllowRemoteActivation, FGameplayAbilitySpecHandle& SpecHandle)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivatePtrs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTag.GetSingleTagContainer(), AbilitiesToActivatePtrs);
	if (AbilitiesToActivatePtrs.Num() != 1)
		return false;

	// Convert from pointers (which can be reallocated, since they point to internal data) to copies of that data
	TArray<FGameplayAbilitySpec> AbilitiesToActivate;
	Algo::Transform(AbilitiesToActivatePtrs, AbilitiesToActivate, [](FGameplayAbilitySpec* SpecPtr) { return *SpecPtr; });

	if (AbilitiesToActivate.Num() != 1)
		return false;
	
	ensure(IsValid(AbilitiesToActivate[0].Ability));
	const bool bSuccess = TryActivateAbility(AbilitiesToActivate[0].Handle, bAllowRemoteActivation);
	if (bSuccess)
		SpecHandle = AbilitiesToActivate[0].Handle;

	return bSuccess;
}

void UITMAbilitySystemComponent::OverrideInitAttributeEffects(const TSubclassOf<UGameplayEffect>& PrimaryGE,
                                                              const TSubclassOf<UGameplayEffect>& VitalGE, const TSubclassOf<UGameplayEffect>& CalculationGE)
{
	InitialPrimaryAttributesEffectClass = PrimaryGE;
	InitialVitalAttributesEffectClass = VitalGE;
	AttributesCalculationEffectClass = CalculationGE;
}

FGameplayEffectContextHandle UITMAbilitySystemComponent::MakeDamageEffectContext(const FAttackData& InAttackData) const
{
	FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(
		FITMAbilitySystemUtilities::AllocEffectContext<FDamageGameplayEffectContext>());
	
	// By default use the owner and avatar as the instigator and causer
	if (ensureMsgf(AbilityActorInfo.IsValid(), TEXT("Unable to make effect context because AbilityActorInfo is not valid.")))
	{
		Context.AddInstigator(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get());
	}

	static_cast<FDamageGameplayEffectContext*>(Context.Get())->AttackData = InAttackData;
	
	return Context;
}
