// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ActivateAbility.generated.h"

struct FBTActivateAbilityMemory
{
	FGameplayAbilitySpecHandle SpecHandle;
};

/**
 * 
 */
UCLASS(DisplayName = "Activate Ability")
class ITM_FRAMEWORK_API UBTT_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ActivateAbility();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UFUNCTION()
	void OnAbilityEnd(const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* OwnerComp);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bEndAbilityOnTaskAborted = true;
};
