// Copyright 2024 Itmwuma

#pragma once

DECLARE_MULTICAST_DELEGATE_FourParams(FOnCharacterHurt, AActor* Character, float DamageValue, bool bIsBlockedHit, bool bIsCriticalHit);

struct FRPGGlobalDelegates
{
	static FOnCharacterHurt OnCharacterHurtDelegate;
};
