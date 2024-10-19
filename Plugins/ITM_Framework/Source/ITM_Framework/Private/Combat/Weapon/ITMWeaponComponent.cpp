// Copyright 2024 Itmwuma


#include "Combat/Weapon/ITMWeaponComponent.h"

#include "GameFramework/Character.h"

UITMWeaponComponent::UITMWeaponComponent()
{
	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UITMWeaponComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (const ACharacter* CharacterOwner = GetOwner<ACharacter>())
	{
		WeaponMeshComp->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
	}
}


void UITMWeaponComponent::DetachWeapon()
{
	if (WeaponMeshComp)
	{
		const FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, true);
		WeaponMeshComp->DetachFromComponent(Rules);

		OnWeaponDetachedDelegate.Broadcast(WeaponMeshComp);
		WeaponMeshComp->DestroyComponent();
	}
}

void UITMWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

