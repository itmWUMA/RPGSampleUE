// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Character/ITMCharacterBase.h"
#include "ITMEnemyCharacterBase.generated.h"

class AITMAIController;
class UBehaviorTree;

/**
 * 
 */
UCLASS(Abstract)
class ITM_FRAMEWORK_API AITMEnemyCharacterBase : public AITMCharacterBase
{
	GENERATED_BODY()

public:
	AITMEnemyCharacterBase();
	
	virtual int32 GetCharacterLevel() const override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BTAsset;

	TWeakObjectPtr<AITMAIController> AIControllerCache = nullptr;
};
