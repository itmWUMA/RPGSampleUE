// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "ITMCharacterBase.generated.h"

class UAttributeSet;

UCLASS(Abstract)
class ITM_FRAMEWORK_API AITMCharacterBase : public ACharacter, public IITMAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AITMCharacterBase();

	// --- IITMAbilitySystemInterface Impl ---
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override { return AttributeSetCache.Get(); }
	// --- IITMAbilitySystemInterface Impl End ---

	virtual int32 GetCharacterLevel() const;

protected:
	virtual void BeginPlay() override;

protected:
	TWeakObjectPtr<UITMAbilitySystemComponent> AbilitySystemComponentCache = nullptr;
	TWeakObjectPtr<UAttributeSet> AttributeSetCache = nullptr;
};
