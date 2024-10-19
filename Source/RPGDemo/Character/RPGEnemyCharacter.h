// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "CharacterDeath.h"
#include "CharacterJobInfo.h"
#include "Character/ITMEnemyCharacterBase.h"
#include "Level/Interaction/Highlightable.h"
#include "RPGEnemyCharacter.generated.h"

class UAwardComponent;
class URPGAttackComponent;
class UITMHurtComponent;
class UITMViewModelBase;
class UWidgetComponent;
class UITMWeaponComponent;
class URPGAttributeSetBase;
/**
 * 
 */
UCLASS()
class RPGDEMO_API ARPGEnemyCharacter : public AITMEnemyCharacterBase,
	public IHighlightable, public ICharacterDeath
{
	GENERATED_BODY()

public:
	ARPGEnemyCharacter();
	
	// --- IHighlightable impl ---
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	// --- IHighlightable impl end ---

	// --- ICharacterDeath impl ---
	virtual void OnDead() override;
	virtual bool IsDead() const override { return bDead; }
	virtual void StartDie() override;
	// --- ICharacterDeath impl end ---

	virtual void OnConstruction(const FTransform& Transform) override;
	
	FORCEINLINE UITMHurtComponent* GetHurtComponent() const { return HurtComp; }
	FORCEINLINE UAwardComponent* GetAwardComponent() const { return AwardComp; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	AActor* GetCombatTarget() const { return CombatTarget.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	void SetCombatTarget(AActor* InCombatTarget);
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget);

	FORCEINLINE FSimpleMulticastDelegate& GetOnEnemyDeadDelegate() { return OnEnemyDeadDelegate; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitAbilityActorInfo() override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDead();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartDie();
	
private:
	void ToggleHighlightImpl(bool bHighlight);
	void InitEnemyStateBar();
	void InitHurt();
	void InitBlackboardValue(AController* AIC);

	void OnHurt(bool bHurt);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITMWeaponComponent> WeaponComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<URPGAttributeSetBase> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> StateBarComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITMHurtComponent> HurtComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<URPGAttackComponent> AttackComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAwardComponent> AwardComp;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UITMViewModelBase> StateBarViewModelClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	float StateBarOffsetHeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	ECharacterJob CharacterJob = ECharacterJob::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	TObjectPtr<UMaterialInstance> DissolveMaterial;

	TWeakObjectPtr<AActor> CombatTarget = nullptr;
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bHighlighted = false;

	float BaseSpeed = 0.f;
	FDelegateHandle OnHurtHandle;

	bool bDead = false;
	FSimpleMulticastDelegate OnEnemyDeadDelegate;
};
