// Copyright 2024 Itmwuma


#include "Core/ITMPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Input/AbilityBindingComponent.h"

AITMPlayerController::AITMPlayerController()
{
	AbilityBindingComp = CreateDefaultSubobject<UAbilityBindingComponent>("AbilityBindingComp");
	
	bReplicates = true;
}

void AITMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (InputSubsystem)
	{
		for (int32 i = 0; i < DefaultMappingContexts.Num(); ++i)
		{
			InputSubsystem->AddMappingContext(DefaultMappingContexts[i], i);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		AbilityBindingComp->BindDefaultAbilities(EnhancedInputComponent);
	}
}