// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindPlayer.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Find Player (Check by distance)")
class RPGDEMO_API UBTS_FindPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_FindPlayer();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override { return TEXT("BTEditor.Graph.BTNode.Decorator.DoesPathExist.Icon"); }
#endif

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CanFollowBlackboardKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PlayerLocationBlackboardKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PlayerBlackboardKey;

	UPROPERTY(EditAnywhere)
	float CheckDistance = 0.f;

	UPROPERTY(EditAnywhere)
	bool bIgnoreZ = false;
};
