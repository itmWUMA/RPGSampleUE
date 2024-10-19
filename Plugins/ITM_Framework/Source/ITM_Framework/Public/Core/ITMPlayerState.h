// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "ITMPlayerState.generated.h"

class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChanged, int32 OldLevel, int32 NewLevel);

/**
 * 
 */
UCLASS(Abstract)
class ITM_FRAMEWORK_API AITMPlayerState : public APlayerState, public IITMAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AITMPlayerState();

	// --- IITMAbilitySystemInterface Impl ---
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override { return nullptr; }
	// --- IITMAbilitySystemInterface Impl End ---

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE void SetPlayerLevel(int32 InLevel) { Level = InLevel; }

	FORCEINLINE FOnPlayerLevelChanged& GetOnPlayerLevelChangedDelegate() { return OnPlayerLevelChanged; }

private:
	UFUNCTION()
	void OnRep_Level(int32 OldValue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	
	FOnPlayerLevelChanged OnPlayerLevelChanged;
};
