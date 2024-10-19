// Copyright 2024 Itmwuma


#include "Core/RPGPlayerController.h"
#include "EnhancedInputComponent.h"
#include "RPGGameMode.h"
#include "GameFramework/Character.h"
#include "Level/Dither/DitherComponent.h"
#include "Level/Interaction/InteractionManager.h"
#include "Level/Load/LoadManager.h"
#include "UI/ITMUserInterfaceUtilities.h"

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (InteractionMgrCache.IsValid())
	{
		TArray<AActor*> HighlightableObjectsCopy = InteractionMgrCache->GetHighlightableObjects();
		for (AActor* HighlightableObject : HighlightableObjectsCopy)
		{
			IHighlightable* Highlightable =Cast<IHighlightable>(HighlightableObject);
			
			// 若发现无效的高光对象，直接取消注册
			if (!IsValid(HighlightableObject) || !Highlightable)
			{
				InteractionMgrCache->UnregisterHighlightable(HighlightableObject);
				continue;
			}
			
			if (CheckActorHighlightEnable(HighlightableObject))
			{
				// 当可高光的对象首次标记为可高光，则高光显示
				InteractionMgrCache->RegisterHighlighted(HighlightableObject);
			}
			else
			{
				// 当已高光的对象首次标记为不可高光，则取消高光
				InteractionMgrCache->UnregisterHighlighted(HighlightableObject);
			}
		}
	}
	
	// Dither效果应用
	const UWorld* World = GetWorld();
	const ACharacter* TargetCharacter = GetCharacter();
	if (IsValid(World) && IsValid(PlayerCameraManager) && IsValid(TargetCharacter))
	{
		TArray<FHitResult> DitherTraceHitResults;
		FCollisionQueryParams CollisionQueryParams;
		World->LineTraceMultiByChannel(DitherTraceHitResults,
			PlayerCameraManager->GetCameraLocation(),
			TargetCharacter->GetActorLocation(),
			DitherTraceChannel, 
			CollisionQueryParams);
		TSet<TWeakObjectPtr<UDitherComponent>> LastFrameDitherComps = CurrentDitherComps;
		for (const FHitResult& DitherTraceHitResult : DitherTraceHitResults)
		{
			if (UDitherComponent* DitherComp = DitherTraceHitResult.GetActor()->GetComponentByClass<UDitherComponent>())
			{
				if (!CurrentDitherComps.Contains(DitherComp))
				{
					// 未加入到缓存的DitherComp表中，则添加并调用FadeOut
					DitherComp->FadeOut();
					CurrentDitherComps.Add(DitherComp);
				}
				else
				{
					// 已添加到缓存的DitherComp表中，则标记缓存DitherComp表已添加（剔除）
					LastFrameDitherComps.Remove(DitherComp);
				}
			}
		}

		// 清除已经无需Dither的Comp
		for (const TWeakObjectPtr<UDitherComponent>& DitherComp : LastFrameDitherComps)
		{
			if (DitherComp.IsValid())
			{
				DitherComp->FadeIn();
				CurrentDitherComps.Remove(DitherComp);
			}
		}
	}
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!World)
		return;

	InteractionMgrCache = World->GetSubsystem<UInteractionManager>();
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::OnMoveActionTriggered);
	EnhancedInputComponent->BindAction(OpenAttributeMenuAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::OnOpenAttributeMenuActionTriggered);
	EnhancedInputComponent->BindAction(SkillModifyAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::OnSkillModifyInputCallback, true);
	EnhancedInputComponent->BindAction(SkillModifyAction, ETriggerEvent::Completed, this, &ARPGPlayerController::OnSkillModifyInputCallback, false);
	EnhancedInputComponent->BindAction(OpenSaveMenuAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::OnOpenSaveMenuTriggered);
}

void ARPGPlayerController::OnMoveActionTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxis = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	
	const FVector ForwardDir = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDir, InputAxis.Y);
		ControlledPawn->AddMovementInput(RightDir, InputAxis.X);
	}
}

void ARPGPlayerController::OnOpenAttributeMenuActionTriggered(const FInputActionValue& InputActionValue)
{
	if (const UWorld* World = GetWorld())
	{
		if (ARPGGameMode* GameMode = World->GetAuthGameMode<ARPGGameMode>())
		{
			GameMode->ShowAttributeMenu();
		}
	}
}

void ARPGPlayerController::OnSkillModifyInputCallback(const FInputActionValue& InputActionValue, bool bTriggered)
{
	bSkillModifyTriggered = bTriggered;
}

void ARPGPlayerController::OnOpenSaveMenuTriggered(const FInputActionValue& InputActionValue)
{
	if (ULoadManager* LoadMgr = ULoadManager::Get(this))
	{
		LoadMgr->ShowSaveUI();
	}
}

bool ARPGPlayerController::CheckActorHighlightEnable(const AActor* InActor) const
{
	if (!InActor)
		return false;
	
	const APawn* OwnerPawn = GetPawn();
	if (!OwnerPawn)
		return false;

	return FVector::DistSquared(InActor->GetActorLocation(), OwnerPawn->GetActorLocation()) <=
		HighlightEnableDistance * HighlightEnableDistance;
}
