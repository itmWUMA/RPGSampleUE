// Copyright 2024 Itmwuma


#include "Character/RPGEnemyCharacter.h"

#include "RPGDemo.h"
#include "AbilitySystem/RPGAttackComponent.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "AbilitySystem/XP/AwardComponent.h"
#include "AI/RPGAIDefines.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Combat/AbilitySystem/Component/ITMHurtComponent.h"
#include "Combat/Weapon/ITMWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/RPGGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ITMViewModelManager.h"
#include "UI/Widget/ITMWidgetBase.h"

ARPGEnemyCharacter::ARPGEnemyCharacter()
{
	WeaponComp = CreateDefaultSubobject<UITMWeaponComponent>("WeaponComp");
	
	AttributeSet = CreateDefaultSubobject<URPGAttributeSetBase>("AttributeSet");
	AttributeSetCache = AttributeSet;
	
	StateBarComp = CreateDefaultSubobject<UWidgetComponent>("StateBarComp");
	StateBarComp->SetupAttachment(RootComponent);
	
	HurtComp = CreateDefaultSubobject<UITMHurtComponent>("HurtComp");

	AttackComp = CreateDefaultSubobject<URPGAttackComponent>("AttackComp");

	AwardComp = CreateDefaultSubobject<UAwardComponent>("AwardComp");

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ARPGEnemyCharacter::HighlightActor()
{
	bHighlighted = true;
	ToggleHighlightImpl(true);
}

void ARPGEnemyCharacter::UnhighlightActor()
{
	bHighlighted = false;
	ToggleHighlightImpl(false);
}

void ARPGEnemyCharacter::OnDead()
{
	bDead = true;
	OnEnemyDeadDelegate.Broadcast();
	BP_OnDead();
}

void ARPGEnemyCharacter::StartDie()
{
	BP_StartDie();
}

void ARPGEnemyCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 初始化状态条
	float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector StateBarDesiredPos =
		GetActorTransform().TransformPosition(FVector(0.f, 0.f, HalfHeight + StateBarOffsetHeight));
	StateBarComp->SetWorldLocation(StateBarDesiredPos);
}

void ARPGEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

void ARPGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	RegisterHighlightActor(this);
	InitEnemyStateBar();
	InitHurt();
}

void ARPGEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterHighlightActor(this);
	HurtComp->GetOnHurtDelegate().Remove(OnHurtHandle);
	
	Super::EndPlay(EndPlayReason);
}

void ARPGEnemyCharacter::InitAbilityActorInfo()
{
	// 若该角色已拥有职业，则直接沿用该职业的初始化参数
	if (CharacterJob != ECharacterJob::None)
	{
		if (const ARPGGameMode* GameMode = Cast<ARPGGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			if (const UCharacterJobInfo* JobInfo = GameMode->GetCharacterJobInfo())
			{
				const FCharacterJobDefaultInfo& DefaultInfo = JobInfo->GetCharacterJobDefaultInfo(CharacterJob);
				AbilitySystemComponent->OverrideInitAttributeEffects(
					DefaultInfo.PrimaryAttributesEffect,
					JobInfo->VitalAttributesEffect,
					JobInfo->AttributeCalculationEffect);
			}
		}
	}
	
	Super::InitAbilityActorInfo();
}

void ARPGEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitBlackboardValue(NewController);
}

void ARPGEnemyCharacter::ToggleHighlightImpl(bool bHighlight)
{
	// 设置角色Mesh的高光
	GetMesh()->SetRenderCustomDepth(bHighlight);
	GetMesh()->SetCustomDepthStencilValue(bHighlight ? CUSTOM_DEPTH_RED : 0);
	
	// 设置角色武器Mesh的高光
	if (USkeletalMeshComponent* WeaponMesh = WeaponComp->GetWeaponMesh())
	{
		WeaponMesh->SetRenderCustomDepth(bHighlight);
		WeaponMesh->SetCustomDepthStencilValue(bHighlight ? CUSTOM_DEPTH_RED : 0);
	}
}

void ARPGEnemyCharacter::InitEnemyStateBar()
{
	if (UITMViewModelManager* ViewModelMgr = UITMViewModelManager::Get(this))
	{
		FString IdStr = TEXT("EnemyStateBar_") + GetName();
		FITMViewModelParams Params;
		Params.ASCCache = GetAbilitySystemComponent();
		Params.AttributeSetCache = GetAttributeSet();
		UITMViewModelBase* ViewModel =
			ViewModelMgr->CreateViewModel(FName(IdStr), StateBarViewModelClass, Params);
		UITMWidgetBase* StateBarWidget = Cast<UITMWidgetBase>(StateBarComp->GetWidget());
		
		if (!StateBarWidget)
		{
			ensureAlwaysMsgf(false, TEXT("Cannot get valid state bar widget!"));
			return;
		}

		StateBarWidget->SetViewModel(ViewModel);
		ViewModel->OnInit();
	}
}

void ARPGEnemyCharacter::InitHurt()
{
	BaseSpeed = GetCharacterMovement()->MaxWalkSpeed;
	OnHurtHandle = HurtComp->GetOnHurtDelegate().AddUObject(this, &ARPGEnemyCharacter::OnHurt);
}

void ARPGEnemyCharacter::InitBlackboardValue(AController* AIC)
{
	if (UBlackboardComponent* BBComp = UAIBlueprintHelperLibrary::GetBlackboard(AIC))
	{
		BBComp->SetValueAsEnum(RPGBBName::CharacterJob, static_cast<uint8>(CharacterJob));
	}
}

void ARPGEnemyCharacter::OnHurt(bool bHurt)
{
	GetCharacterMovement()->MaxWalkSpeed = bHurt ? 0.f : BaseSpeed;
}
