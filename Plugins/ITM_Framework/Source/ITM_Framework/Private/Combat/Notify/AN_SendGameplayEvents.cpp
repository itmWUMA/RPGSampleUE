// Copyright 2024 Itmwuma


#include "Combat/Notify/AN_SendGameplayEvents.h"

#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"

void UAN_SendGameplayEvents::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetWorld())
		return;

	if (const IITMAbilitySystemInterface* ASI = Cast<IITMAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			for (const FSendGameplayEventData& Data : EventsData)
			{
				FGameplayEventData Payload;
				Payload.InstigatorTags = Data.PayloadInstigator;
				Payload.TargetTags = Data.PayloadTarget;
				Payload.EventMagnitude = Data.PayloadMagnitude;
				ASC->HandleGameplayEvent(Data.EventTag, &Payload);
			}
		}
	}
}
