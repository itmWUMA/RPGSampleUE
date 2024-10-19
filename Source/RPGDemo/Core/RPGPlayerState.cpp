// Copyright 2024 Itmwuma


#include "Core/RPGPlayerState.h"

#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Level/Load/RPGSaveGame.h"
#include "Net/UnrealNetwork.h"

ARPGPlayerState::ARPGPlayerState()
{
	AttributeSet = CreateDefaultSubobject<URPGAttributeSetBase>("AttributeSet");
}

void ARPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGPlayerState, XP);
}

UAttributeSet* ARPGPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void ARPGPlayerState::SetXP(int32 InXP)
{
	const int32 OldXP = XP;
	XP = InXP;
	OnXPChanged.Broadcast(OldXP, XP);
}

void ARPGPlayerState::AddToXP(int32 InXP)
{
	const int32 OldXP = XP;
	XP += InXP;
	OnXPChanged.Broadcast(OldXP, XP);
}

void ARPGPlayerState::AddToAttributePoint(int32 InAttributePoint)
{
	const int32 OldAttributePoint = AttributePoint;
	AttributePoint += InAttributePoint;
	OnAttributePointChanged.Broadcast(OldAttributePoint, AttributePoint);
}

void ARPGPlayerState::InitPlayerStateInfoFromSaveGame(const URPGSaveGame* InSaveGame)
{
	if (!IsValid(InSaveGame))
		return;
	
	Level = InSaveGame->PlayerLevel;
	XP = InSaveGame->XP;
	AttributePoint = InSaveGame->AttributePoint;
}

void ARPGPlayerState::OnRep_XP(int32 OldXP)
{
}

void ARPGPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
}
