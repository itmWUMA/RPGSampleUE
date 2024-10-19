// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SendGameplayEvents.generated.h"

USTRUCT()
struct FSendGameplayEventData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag = FGameplayTag::EmptyTag;
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer PayloadInstigator = FGameplayTagContainer::EmptyContainer;
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer PayloadTarget = FGameplayTagContainer::EmptyContainer;
	UPROPERTY(EditAnywhere)
	float PayloadMagnitude = 0.f;
};

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UAN_SendGameplayEvents : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FSendGameplayEventData> EventsData;
};
