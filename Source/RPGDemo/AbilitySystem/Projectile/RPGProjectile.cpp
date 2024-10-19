// Copyright 2024 Itmwuma


#include "AbilitySystem/Projectile/RPGProjectile.h"

#include "RPGDemo.h"
#include "AbilitySystem/RPGAttackComponent.h"
#include "Combat/AbilitySystem/Component/ITMAttackComponent.h"

ARPGProjectile::ARPGProjectile()
{
	AttackComp = CreateDefaultSubobject<URPGAttackComponent>("AttackComp");
}

void ARPGProjectile::OnLaunchObjectRegistered()
{
	Super::OnLaunchObjectRegistered();

	for (UPrimitiveComponent* PrimitiveComp : OverlapComps)
	{
		PrimitiveComp->SetCollisionObjectType(ECC_PROJECTILE);
	}
}

void ARPGProjectile::SetAttackData(const FAttackData& Data)
{
	AttackData = Data;
}

void ARPGProjectile::OnLaunchObjectOverlapped(AActor* TargetActor, const FHitResult& SweepResult)
{
	Super::OnLaunchObjectOverlapped(TargetActor, SweepResult);

	AttackComp->Attack(TargetActor, AttackData);
}
