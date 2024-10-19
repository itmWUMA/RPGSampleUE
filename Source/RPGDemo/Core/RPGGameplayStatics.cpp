// Copyright 2024 Itmwuma


#include "Core/RPGGameplayStatics.h"

#include "RPGGameMode.h"
#include "Kismet/GameplayStatics.h"

const UCurveTable* URPGGameplayStatics::GetEffectCoefficients(const UObject* WorldContext)
{
	if (const ARPGGameMode* GM = Cast<ARPGGameMode>(UGameplayStatics::GetGameMode(WorldContext)))
	{
		return GM->GetEffectCoefficients();
	}
	return nullptr;
}
