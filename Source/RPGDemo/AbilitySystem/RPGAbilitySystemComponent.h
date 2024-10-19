// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

class URPGSaveGame;

/**
 * 
 */
UCLASS()
class RPGDEMO_API URPGAbilitySystemComponent : public UITMAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE const FGameplayTag& GetHurtAbilityTag() const { return HurtAbilityTag; }
	FORCEINLINE const FGameplayTag& GetDieAbilityTag() const { return DieAbilityTag; }

	virtual FGameplayEffectContextHandle MakeDamageEffectContext(const FAttackData& InAttackData) const override;

	void UpgradeAttribute(const FGameplayAttribute& InAttribute);

	void InitAttributesFromSaveGame(URPGSaveGame* InSaveGame);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FGameplayTag HurtAbilityTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FGameplayTag DieAbilityTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP")
	TSubclassOf<UGameplayEffect> UpgradeAttributeEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP", meta = (EditCondition = "UpgradeAttributeEffectClass != nullptr"))
	FGameplayTagContainer UpgradeAttributeTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Attribute")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesFromSaveDataEffect;
};
