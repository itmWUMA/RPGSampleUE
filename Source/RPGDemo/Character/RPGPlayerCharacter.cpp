// Copyright 2024 Itmwuma


#include "Character/RPGPlayerCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Combat/Weapon/ITMWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/RPGHUD.h"
#include "Core/RPGPlayerController.h"
#include "Core/RPGPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Load/LoadManager.h"
#include "Level/Load/RPGSaveGame.h"
#include "UI/ITMUserInterfaceUtilities.h"

ARPGPlayerCharacter::ARPGPlayerCharacter()
{
	if (UCharacterMovementComponent* CharMovementComp = GetCharacterMovement())
	{
		CharMovementComp->bOrientRotationToMovement = true;
		CharMovementComp->bConstrainToPlane = true;
		CharMovementComp->bSnapToPlaneAtStart = true;
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	WeaponComp = CreateDefaultSubobject<UITMWeaponComponent>("WeaponComp");

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ARPGPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>())
	{
		PS->GetOnPlayerLevelChangedDelegate().AddUObject(this, &ARPGPlayerCharacter::OnLevelChanged);
	}
}

void ARPGPlayerCharacter::UnPossessed()
{
	if (ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>())
	{
		PS->GetOnPlayerLevelChangedDelegate().RemoveAll(this);
	}
	
	Super::UnPossessed();
}

void ARPGPlayerCharacter::InitHUD()
{
	Super::InitHUD();

	if (const ARPGPlayerController* PC = GetController<ARPGPlayerController>())
	{
		if (ARPGHUD* HUD = PC->GetHUD<ARPGHUD>())
		{
			const FITMViewModelParams Params = FITMUserInterfaceUtilities::MakeDefaultViewModelParams(this);
			HUD->InitHUD(Params);
		}
	}
}

void ARPGPlayerCharacter::InitAbilityActorInfo()
{
	if (ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>())
	{
		if (URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent()))
		{
			ASC->InitAbilityActorInfo(PS, this);

			if (ULoadManager* LoadMgr = ULoadManager::Get(this))
			{
				URPGSaveGame* CurSaveData = LoadMgr->LoadSlotData(LoadMgr->GetCurrentSaveSlotIndex());
				if (IsValid(CurSaveData))
				{
					ASC->InitAttributesFromSaveGame(CurSaveData);
				}
				else
				{
					ASC->InitAttributes();
				}
			}
			
			AbilitySystemComponentCache = ASC;
			AttributeSetCache = PS->GetAttributeSet();

			ASC->AbilityActorInfoSet();

			if (ULoadManager* LoadMgr = ULoadManager::Get(this))
				PS->InitPlayerStateInfoFromSaveGame(LoadMgr->LoadSlotData(LoadMgr->GetCurrentSaveSlotIndex()));
			
			InitHUD();
		}
	}
}

void ARPGPlayerCharacter::OnLevelChanged(int32 OldLevel, int32 NewLevel)
{
	if (OldLevel >= NewLevel)
		return;

	const FVector ActorLocation = GetActorLocation();
	const FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
	
	
	UGameplayStatics::PlaySoundAtLocation(this,
		LevelUpEffectData.SoundAsset,
		ActorLocation,
		FRotator::ZeroRotator);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
		LevelUpEffectData.ParticleAsset,
		ActorLocation,
		(ActorLocation - CameraLocation).ToOrientationRotator());
}
