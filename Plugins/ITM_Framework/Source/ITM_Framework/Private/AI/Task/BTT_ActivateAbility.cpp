// Copyright 2024 Itmwuma


#include "AI/Task/BTT_ActivateAbility.h"

#include "AIController.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"

UBTT_ActivateAbility::UBTT_ActivateAbility()
{
	NodeName = TEXT("Activate Ability");
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTT_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const APawn* Pawn = OwnerComp.GetOwner<AAIController>()->GetPawn())
	{
		if (UITMAbilitySystemComponent* ASC = Pawn->GetComponentByClass<UITMAbilitySystemComponent>())
		{
			FGameplayAbilitySpecHandle SpecHandle;
			if (ASC->TryActivateSingleAbilityByTag(AbilityTag, true, SpecHandle))
			{
				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(SpecHandle);
				check(Spec);
				
				if (FBTActivateAbilityMemory* ActivateAbilityMemory = CastInstanceNodeMemory<FBTActivateAbilityMemory>(NodeMemory))
				{
					ActivateAbilityMemory->SpecHandle = SpecHandle;
				}
				
				if (UGameplayAbility* CurrentGA = Spec->GetPrimaryInstance())
				{
					CurrentGA->OnGameplayAbilityEndedWithData.AddUFunction(this,
						TEXT("OnAbilityEnd"), &OwnerComp);
					return EBTNodeResult::InProgress;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

FString UBTT_ActivateAbility::GetStaticDescription() const
{
	return AbilityTag.ToString();
}

uint16 UBTT_ActivateAbility::GetInstanceMemorySize() const
{
	return sizeof(FBTActivateAbilityMemory);
}

void UBTT_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                          EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	APawn* Pawn = OwnerComp.GetOwner<AAIController>()->GetPawn();
	if (!IsValid(Pawn))
		return;

	switch (TaskResult) {
	case EBTNodeResult::Succeeded:
		break;
	case EBTNodeResult::Failed:
		break;
	case EBTNodeResult::Aborted:
		{
			if (UITMAbilitySystemComponent* ASC = Pawn->GetComponentByClass<UITMAbilitySystemComponent>();
				IsValid(ASC) && bEndAbilityOnTaskAborted)
			{
				if (FBTActivateAbilityMemory* ActivateAbilityMemory = CastInstanceNodeMemory<FBTActivateAbilityMemory>(NodeMemory))
				{
					FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(ActivateAbilityMemory->SpecHandle);
					if (!Spec)
						return;
					if (UITMGameplayAbility* CurrentGA = Cast<UITMGameplayAbility>(Spec->GetPrimaryInstance());
						IsValid(CurrentGA) && CurrentGA->AbilityTags.HasTagExact(AbilityTag))
					{
						CurrentGA->TryEndAbility();
					}
				}
			}
		}
		break;
	case EBTNodeResult::InProgress:
		break;
	default: ;
	}
}

void UBTT_ActivateAbility::OnAbilityEnd(const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* OwnerComp)
{
	if (AbilityTag.IsValid() && AbilityEndedData.AbilityThatEnded->AbilityTags.HasTagExact(AbilityTag))
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

