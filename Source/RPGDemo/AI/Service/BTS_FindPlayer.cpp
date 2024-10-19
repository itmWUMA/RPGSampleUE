// Copyright 2024 Itmwuma


#include "AI/Service/BTS_FindPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTS_FindPlayer::UBTS_FindPlayer()
{
	bNotifyTick = true;
	CanFollowBlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_FindPlayer, CanFollowBlackboardKey));
	PlayerLocationBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_FindPlayer, PlayerLocationBlackboardKey));
	PlayerBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_FindPlayer, PlayerBlackboardKey), AActor::StaticClass());
}

void UBTS_FindPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		CanFollowBlackboardKey.ResolveSelectedKey(*BBAsset);
		PlayerLocationBlackboardKey.ResolveSelectedKey(*BBAsset);
		PlayerBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTS_FindPlayer::GetStaticDescription() const
{
	FString TargetString = TEXT("CheckDistance");
	if (bIgnoreZ)
		TargetString.Append(TEXT("(XY)"));
	return FString::Printf(TEXT("%s: %.2f"), *TargetString, CheckDistance);
}

void UBTS_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(OwnerComp.GetOwner(), 0))
	{
		const FVector PlayerLocation = Player->GetActorLocation();
		const FVector OwnerLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
		const FVector PlayerToOwner = PlayerLocation - OwnerLocation;
		const bool bFindPlayer = (bIgnoreZ ? FVector2D(PlayerToOwner).Length() : PlayerToOwner.Length()) < CheckDistance;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(CanFollowBlackboardKey.SelectedKeyName, bFindPlayer);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(PlayerLocationBlackboardKey.SelectedKeyName, PlayerLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerBlackboardKey.SelectedKeyName, Player);
	}
}
