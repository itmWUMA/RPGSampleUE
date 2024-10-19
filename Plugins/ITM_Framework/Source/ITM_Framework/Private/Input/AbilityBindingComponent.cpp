// Copyright 2024 Itmwuma


#include "Input/AbilityBindingComponent.h"

#include "EnhancedInputComponent.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"
#include "Core/ITMPlayerController.h"
#include "Input/AbilityInputBindingObject.h"
#include "Input/ITMInputConfig.h"

UAbilityBindingComponent::UAbilityBindingComponent()
{
}

void UAbilityBindingComponent::BindAbilityInput(UEnhancedInputComponent* InputComponent,
	FGameplayAbilitySpec& Spec)
{
	const FGameplayTag DefaultInputTag = GetDefaultInputTag(ASCCache.Get(), Spec);
	BindAbilityInput(InputComponent, DefaultInputTag, Spec);
}

void UAbilityBindingComponent::BindAbilityInput(UEnhancedInputComponent* InputComponent, const FGameplayTag& InputTag,
	FGameplayAbilitySpec& Spec)
{
	if (!InputComponent ||
		!InputConfig)
	{
		return;
	}

	// Config中查找InputActionData
	const FITMInputActionData* ActionDataPtr = InputConfig->AbilityInputActions.Find(InputTag);
	if (!ActionDataPtr || !ActionDataPtr->InputAction)
		return;

	const FITMInputActionData ActionData = *ActionDataPtr;
	// TODO: 是否可以直接使用DataAsset的绑定策略对象实例？
	UAbilityInputBindingObject* BindingPolicy = DuplicateObject<UAbilityInputBindingObject>(ActionData.AbilityBindingObject, this);
	BindingPolicy->ASCCache = ASCCache;
	if (BindingPolicy)
	{
		BindingDataMap.Add(InputTag, {BindingPolicy, Spec.Handle});

		InputComponent->BindAction(ActionData.InputAction, ETriggerEvent::Triggered, this,
			&UAbilityBindingComponent::AbilityInputBinding_Triggered, InputTag);

		InputComponent->BindAction(ActionData.InputAction, ETriggerEvent::Started, this,
			&UAbilityBindingComponent::AbilityInputBinding_Started, InputTag);
		
		InputComponent->BindAction(ActionData.InputAction, ETriggerEvent::Ongoing, this,
			&UAbilityBindingComponent::AbilityInputBinding_Ongoing, InputTag);
		
		InputComponent->BindAction(ActionData.InputAction, ETriggerEvent::Completed, this,
			&UAbilityBindingComponent::AbilityInputBinding_Completed, InputTag);
		
		InputComponent->BindAction(ActionData.InputAction, ETriggerEvent::Canceled, this,
			&UAbilityBindingComponent::AbilityInputBinding_Canceled, InputTag);
	}

	// AbilityTag需要记录当前的InputTag
	Spec.DynamicAbilityTags.AddTag(InputTag);
}

void UAbilityBindingComponent::BindDefaultAbilities(UEnhancedInputComponent* InputComponent)
{
	if (!ASCCache.IsValid())
		return;

	TArray<FGameplayAbilitySpec>& ActivatableAbilities = ASCCache->GetActivatableAbilities();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities)
	{
		BindAbilityInput(InputComponent, Spec);
	}
}

bool UAbilityBindingComponent::GetBindingDataWrapperByTag(const FGameplayTag& InputTag, FAbilityBindingDataWrapper& OutPolicyData)
{
	if (const FAbilityBindingDataWrapper* Data = BindingDataMap.Find(InputTag))
	{
		OutPolicyData = *Data;
		return true;
	}
	return false;
}

FGameplayTag UAbilityBindingComponent::GetDefaultInputTag(UAbilitySystemComponent* InASC,
                                                          const FGameplayAbilitySpec& InSpec)
{
	if (!InASC)
		return FGameplayTag::EmptyTag;

	if (UITMGameplayAbility* AbilityCDO = Cast<UITMGameplayAbility>(InSpec.Ability))
	{
		return AbilityCDO->GetDefaultInputTag();
	}
	
	return FGameplayTag::EmptyTag;
}


void UAbilityBindingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AITMPlayerController* Owner = GetOwner<AITMPlayerController>())
	{
		if (IITMAbilitySystemInterface* TargetPawn = Cast<IITMAbilitySystemInterface>(Owner->GetPawn()))
		{
			ASCCache = TargetPawn->GetAbilitySystemComponent();
		}
	}
}

void UAbilityBindingComponent::AbilityInputBinding_Triggered(const FInputActionValue& Value, const FGameplayTag InInputTag)
{
	if (FAbilityBindingDataWrapper* Data = BindingDataMap.Find(InInputTag))
	{
		if (Data->BindingObject)
		{
			Data->BindingObject->OnTriggered(Value, InInputTag, Data->SpecHandle);
		}
	}
}

void UAbilityBindingComponent::AbilityInputBinding_Started(const FInputActionValue& Value,
	const FGameplayTag InInputTag)
{
	if (FAbilityBindingDataWrapper* Data = BindingDataMap.Find(InInputTag))
	{
		if (Data->BindingObject)
		{
			Data->BindingObject->OnStarted(Value, InInputTag, Data->SpecHandle);
		}
	}
}

void UAbilityBindingComponent::AbilityInputBinding_Ongoing(const FInputActionValue& Value,
	const FGameplayTag InInputTag)
{
	if (FAbilityBindingDataWrapper* Data = BindingDataMap.Find(InInputTag))
	{
		if (Data->BindingObject)
		{
			Data->BindingObject->OnGoing(Value, InInputTag, Data->SpecHandle);
		}
	}
}

void UAbilityBindingComponent::AbilityInputBinding_Completed(const FInputActionValue& Value,
	const FGameplayTag InInputTag)
{
	if (FAbilityBindingDataWrapper* Data = BindingDataMap.Find(InInputTag))
	{
		if (Data->BindingObject)
		{
			Data->BindingObject->OnCompleted(Value, InInputTag, Data->SpecHandle);
		}
	}
}

void UAbilityBindingComponent::AbilityInputBinding_Canceled(const FInputActionValue& Value,
	const FGameplayTag InInputTag)
{
	if (FAbilityBindingDataWrapper* Data = BindingDataMap.Find(InInputTag))
	{
		if (Data->BindingObject)
		{
			Data->BindingObject->OnCanceled(Value, InInputTag, Data->SpecHandle);
		}
	}
}
