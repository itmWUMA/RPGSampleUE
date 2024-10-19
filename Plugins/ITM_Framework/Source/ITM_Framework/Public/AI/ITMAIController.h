// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ITMAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API AITMAIController : public AAIController
{
	GENERATED_BODY()

public:
	AITMAIController();

	void InitAI(UBehaviorTree* InBTAsset);
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BTComp;
};
