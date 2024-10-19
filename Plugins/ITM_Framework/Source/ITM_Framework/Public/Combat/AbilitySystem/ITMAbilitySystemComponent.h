// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ITMAbilityDefines.h"
#include "ITMAbilitySystemComponent.generated.h"

class UITMGameplayAbility;

UINTERFACE()
class UITMAbilitySystemInterface : public UAbilitySystemInterface
{
	GENERATED_BODY()
};

class IITMAbilitySystemInterface : public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual UAttributeSet* GetAttributeSet() const = 0;
};

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	FORCEINLINE FOnAssetTagsApplied& GetOnAssetTagsApplied() { return AssetTagsAppliedDg; }
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual void InitAttributes();
	virtual void GrantAbilities();
	virtual bool TryActivateSingleAbilityByTag(const FGameplayTag& AbilityTag, bool bAllowRemoteActivation, FGameplayAbilitySpecHandle& SpecHandle);

	void OverrideInitAttributeEffects(const TSubclassOf<UGameplayEffect>& PrimaryGE, const TSubclassOf<UGameplayEffect>& VitalGE, const TSubclassOf<UGameplayEffect>& CalculationGE);

	virtual FGameplayEffectContextHandle MakeDamageEffectContext(const FAttackData& InAttackData) const;

protected:
	UFUNCTION(Client, Reliable)
	virtual void ClientOnEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle EffectHandle);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Attribute")
	TSubclassOf<UGameplayEffect> InitialPrimaryAttributesEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Attribute")
	TSubclassOf<UGameplayEffect> InitialVitalAttributesEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Attribute")
	TSubclassOf<UGameplayEffect> AttributesCalculationEffectClass;

	UPROPERTY(EditAnywhere, Category = "GAS|Ability")
	TArray<TSubclassOf<UITMGameplayAbility>> DefaultAbilities;
	
	FOnAssetTagsApplied AssetTagsAppliedDg;
	
	FDelegateHandle OnEffectAppliedHandle;
};
