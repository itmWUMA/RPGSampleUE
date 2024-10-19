// Copyright 2024 Itmwuma


#include "AbilitySystem/Ability/GA_Summon.h"

#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/SummonHandleComponent.h"
#include "AbilitySystem/Entity/EntityTransformGenerator.h"
#include "Character/RPGEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UGA_Summon::StartSummon()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(AvatarActor, 0);
	if (!AvatarActor || !AvatarActor->GetWorld() || !PlayerCharacter)
		return;
	
	TArray<FVector> TargetLocations = GetSpawnLocations();
	if (TargetLocations.IsEmpty())
		return;

	if (SummonInterval > 0.f)
	{
		SummonActorInternal(TargetLocations[0], PlayerCharacter, AvatarActor);
		SummonTimerHandle.SetNumZeroed(TargetLocations.Num());
		for (int32 i = 1; i < TargetLocations.Num(); ++i)
		{
			FVector TargetLocation = TargetLocations[i];
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this, WeakCharacter = MakeWeakObjectPtr<ACharacter>(PlayerCharacter)](FVector Location)
			{
				if (WeakCharacter.IsValid())
					SummonActorInternal(Location, WeakCharacter.Get(), GetAvatarActorFromActorInfo());
			}, TargetLocation);
			AvatarActor->GetWorld()->GetTimerManager().SetTimer(SummonTimerHandle[i], TimerDelegate, SummonInterval * i, false);
		}
	}
	else
	{
		for (const FVector& TargetLocation : TargetLocations)
		{
			SummonActorInternal(TargetLocation, PlayerCharacter, AvatarActor);
		}
	}
}

void UGA_Summon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Summon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		for (FTimerHandle& TimerHandle : SummonTimerHandle)
        {
			ActorInfo->AvatarActor->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }
		SummonTimerHandle.Empty();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_Summon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bCanSummonWhenSummonedExist || !ActorInfo)
		return bResult;

	if (USummonHandleComponent* SummonHandler = ActorInfo->AvatarActor->FindComponentByClass<USummonHandleComponent>())
	{
		TArray<TWeakObjectPtr<AActor>> CachedRegisteredSummonActors;
		SummonHandler->GetRegisteredSummonActorByAbilityTag(AbilityTags.First(), CachedRegisteredSummonActors);
		return bResult && CachedRegisteredSummonActors.IsEmpty();
	}

	return bResult;
}

TArray<FVector> UGA_Summon::GetSpawnLocations_Implementation() const
{
	if (!TransformGenerator)
		return TArray<FVector>();

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const UWorld* World = AvatarActor->GetWorld();
	const FEntityTransformGenerateData GenerateData = {MakeWeakObjectPtr<AActor>(AvatarActor)};

	TArray<FVector> TargetLocations;
	for (int32 i = 0; i < SpawnPointCount; ++i)
	{
		FTransform TargetTransform = TransformGenerator->GetTargetTransforms(GenerateData, i);
		FVector TargetLocation = TargetTransform.GetLocation();
		if (bSummonOnGround)
		{
			FHitResult HitResult;
			World->LineTraceSingleByChannel(
				HitResult,
				TargetLocation + FVector(0.f, 0.f, TraceDistance),
				TargetLocation + FVector(0.f, 0.f, -TraceDistance),
				LineTraceChannel);
			if (HitResult.bBlockingHit)
				TargetLocation = HitResult.Location;
		}
		TargetLocations.Push(TargetLocation);
	}

	return TargetLocations;
}

void UGA_Summon::SummonActorInternal(const FVector& TargetLocation, const ACharacter* PlayerCharacter, AActor* AvatarActor)
{
	if (!SummonedClass)
		return;
	
	if (SummonParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(PlayerCharacter, SummonParticle, TargetLocation);
	}

	FVector FinalLocation = TargetLocation;
	if (const ACharacter* CharacterCDO = Cast<ACharacter>(SummonedClass.GetDefaultObject()))
	{
		FinalLocation.Z = CharacterCDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	
	const FVector FacingTarget = PlayerCharacter->GetActorLocation() - FinalLocation;
	const FRotator FacingTargetRotation = FVector(FacingTarget.X, FacingTarget.Y, 0.f).Rotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Cast<APawn>(AvatarActor);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* SummonedActor = AvatarActor->GetWorld()->SpawnActor(SummonedClass, &FinalLocation, &FacingTargetRotation, SpawnParams);
	if (!SummonedActor)
		return;
	
	if (APawn* SummonedPawn = Cast<APawn>(SummonedActor))
	{
		SummonedPawn->SpawnDefaultController();
	}

	if (USummonHandleComponent* SummonHandler = AvatarActor->FindComponentByClass<USummonHandleComponent>())
	{
		SummonHandler->RegisterSummonedActor(AbilityTags.First(), SummonedActor);
	}
	
	SummonedActorCache.Push(SummonedActor);
}
