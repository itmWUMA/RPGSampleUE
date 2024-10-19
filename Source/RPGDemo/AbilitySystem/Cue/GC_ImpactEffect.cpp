// Copyright 2024 Itmwuma


#include "AbilitySystem/Cue/GC_ImpactEffect.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

bool UGC_ImpactEffect::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(MyTarget,
		ImpactEffect.SoundAsset,
		Parameters.Location,
		FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(MyTarget,
		ImpactEffect.ParticleAsset,
		Parameters.Location);
	
	return true;
}
