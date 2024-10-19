// Copyright 2024 Itmwuma


#include "AI/ITMAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AITMAIController::AITMAIController()
{
	BTComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BTComp");
}

void AITMAIController::InitAI(UBehaviorTree* InBTAsset)
{
	if (!InBTAsset)
		return;
	
	RunBehaviorTree(InBTAsset);
	GetBlackboardComponent()->InitializeBlackboard(*InBTAsset->BlackboardAsset);
}
