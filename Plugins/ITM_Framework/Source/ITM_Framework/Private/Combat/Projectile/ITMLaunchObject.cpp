// Copyright 2024 Itmwuma


#include "Combat/Projectile/ITMLaunchObject.h"

#include "NiagaraFunctionLibrary.h"
#include "Combat/Projectile/LaunchObjectMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/ITMFrameworkSettings.h"

AITMLaunchObject::AITMLaunchObject()
{
	MovementComp = CreateDefaultSubobject<ULaunchObjectMovementComponent>("MovementComp");
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AITMLaunchObject::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RegisterOverlapComponents();
}

void AITMLaunchObject::BeginPlay()
{
	Super::BeginPlay();
	
	OnLaunchObjectLaunched();
}

void AITMLaunchObject::OnLaunchObjectRegistered()
{
	BP_OnLaunchObjectRegistered();
}

void AITMLaunchObject::OnLaunchObjectLaunched()
{
	BP_OnLaunchObjectLaunched();
}

void AITMLaunchObject::OnLaunchObjectDestroyed()
{
	UGameplayStatics::PlaySoundAtLocation(this,
		ImpactEffectData.SoundAsset,
		GetActorLocation(),
		FRotator::ZeroRotator);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
		ImpactEffectData.ParticleAsset,
		GetActorLocation());

	BP_OnLaunchObjectDestroyed();
	
	if (HasAuthority())
	{
		SetLifeSpan(0.01f);
	}
}

void AITMLaunchObject::OnLaunchObjectOverlapped(AActor* TargetActor, const FHitResult& SweepResult)
{
	BP_OnLaunchObjectOverlapped(TargetActor, SweepResult);
}

void AITMLaunchObject::RegisterOverlapComponents()
{
	const UWorld* World = GetWorld();
	if (!World)
		return;

	// 在编辑器预览环境下，清除多余的组件缓存
	if (World->WorldType != EWorldType::Game && World->WorldType != EWorldType::PIE)
	{
		for (UActorComponent* Comp : OverlapComps)
		{
			Comp->DestroyComponent();
		}
		OverlapComps.Empty();
	}
	
	const FName OverlapTag = UITMFrameworkSettings::Get()->LaunchObjectOverlapTag;
	TArray<UActorComponent*> Comps = GetComponentsByTag(UPrimitiveComponent::StaticClass(), OverlapTag);
	for (UActorComponent* Comp : Comps)
	{
		if (UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Comp))
		{
			OverlapComps.Add(PrimitiveComp);
			PrimitiveComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			PrimitiveComp->OnComponentBeginOverlap.AddDynamic(this, &AITMLaunchObject::OnBeginOverlap);
		}
	}

	OnLaunchObjectRegistered();
}

bool AITMLaunchObject::CanOverlap(const AActor* TargetActor) const
{
	if (!TargetActor)
		return false;
	
	for (const TSubclassOf<AActor>& IgnoreActor : IgnoreActorsType)
	{
		if (TargetActor->IsA(IgnoreActor))
		{
			return false;
		}
	}
	
	return true;
}

void AITMLaunchObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanOverlap(OtherActor))
		return;

	OnLaunchObjectOverlapped(OtherActor, SweepResult);
	
	if (bDestroyWhenOverlap)
	{
		for (UPrimitiveComponent* OverlapComp : OverlapComps)
		{
			OverlapComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		OnLaunchObjectDestroyed();
	}
}
