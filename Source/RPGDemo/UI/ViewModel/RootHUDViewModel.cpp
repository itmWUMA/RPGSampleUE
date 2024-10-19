// Copyright 2024 Itmwuma


#include "UI/ViewModel/RootHUDViewModel.h"

#include "AbilitySystem/RPGAttributeSetBase.h"
#include "AbilitySystem/XP/LevelUpInfoDataAsset.h"
#include "Combat/AbilitySystem/ITMAbilitySystemComponent.h"
#include "Core/FRPGGlobalDelegates.h"
#include "Core/RPGPlayerState.h"
#include "Kismet/GameplayStatics.h"

void URootHUDViewModel::BroadcastInitialValues()
{
	OnInitAttributeValues();
}

void URootHUDViewModel::BindCallbackToDependencies()
{
	if (!Params.ASCCache || !Params.PlayerStateCache)
		return;

	if (const URPGAttributeSetBase* RPGAttributeSet = Cast<URPGAttributeSetBase>(Params.AttributeSetCache))
	{
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetHealthAttribute())
			.AddUObject(this, &URootHUDViewModel::OnHealthChanged);
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxHealthAttribute())
			.AddUObject(this, &URootHUDViewModel::OnMaxHealthChanged);
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetManaAttribute())
			.AddUObject(this, &URootHUDViewModel::OnManaChanged);
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxManaAttribute())
			.AddUObject(this, &URootHUDViewModel::OnMaxManaChanged);

		if (UITMAbilitySystemComponent* ASC = Cast<UITMAbilitySystemComponent>(Params.ASCCache))
		{
			ASC->GetOnAssetTagsApplied().AddUObject(this, &URootHUDViewModel::OnAssetTagsApplied);
		}

		if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(Params.PlayerStateCache))
		{
			PS->GetOnXPChangedDelegate().AddUObject(this, &URootHUDViewModel::OnXPChanged);
			PS->GetOnPlayerLevelChangedDelegate().AddUObject(this, &URootHUDViewModel::OnPlayerLevelChanged);
		}
	}

	OnCharacterHurtHandle =
		FRPGGlobalDelegates::OnCharacterHurtDelegate.AddUObject(this, &URootHUDViewModel::OnCharacterApplyDamage);
}

void URootHUDViewModel::UnbindCallbackToDependencies()
{
	if (!Params.ASCCache || !Params.PlayerStateCache)
		return;

	if (const URPGAttributeSetBase* RPGAttributeSet = Cast<URPGAttributeSetBase>(Params.AttributeSetCache))
	{
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetHealthAttribute()).RemoveAll(this);
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxHealthAttribute()).RemoveAll(this);
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetManaAttribute()).RemoveAll(this);
		Params.ASCCache->GetGameplayAttributeValueChangeDelegate(RPGAttributeSet->GetMaxManaAttribute()).RemoveAll(this);

		if (UITMAbilitySystemComponent* ASC = Cast<UITMAbilitySystemComponent>(Params.ASCCache))
		{
			ASC->GetOnAssetTagsApplied().RemoveAll(this);
		}

		if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(Params.PlayerStateCache))
		{
			PS->GetOnXPChangedDelegate().RemoveAll(this);
			PS->GetOnPlayerLevelChangedDelegate().RemoveAll(this);
		}
	}

	FRPGGlobalDelegates::OnCharacterHurtDelegate.Remove(OnCharacterHurtHandle);
}

void URootHUDViewModel::OnInit()
{
	Super::OnInit();

	if (MessageDataTable)
	{
		TArray<FName> RowNames = MessageDataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			if (const FTagUIWidgetRow* Row = MessageDataTable->FindRow<FTagUIWidgetRow>(RowName, TEXT("TagUIWidgetRow")))
			{
				MessageDataTableTagMap.Add(Row->MessageTag, RowName);
			}
		}
	}
}

void URootHUDViewModel::OnInitAttributeValues()
{
	if (!Params.AttributeSetCache)
		return;
	
	if (const URPGAttributeSetBase* RPGAttributeSet = Cast<URPGAttributeSetBase>(Params.AttributeSetCache))
	{
		OnHealthChangedDg.Broadcast(RPGAttributeSet->GetHealth());
		OnMaxHealthChangedDg.Broadcast(RPGAttributeSet->GetMaxHealth());
		OnManaChangedDg.Broadcast(RPGAttributeSet->GetMana());
		OnMaxManaChangedDg.Broadcast(RPGAttributeSet->GetMaxMana());
	}

	if (const ARPGPlayerState* PS = Cast<ARPGPlayerState>(Params.PlayerStateCache))
	{
		const int32 CurXP = PS->GetXP();
		OnXPPercentChangedDg.Broadcast(GetXPPercent(CurXP));
		const int32 CurLevel = PS->GetPlayerLevel();
		OnPlayerLevelUpgradeDg.Broadcast(true, CurLevel, CurLevel);
	}
}

void URootHUDViewModel::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChangedDg.Broadcast(Data.NewValue);
}

void URootHUDViewModel::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChangedDg.Broadcast(Data.NewValue);
}

void URootHUDViewModel::OnManaChanged(const FOnAttributeChangeData& Data)
{
	OnManaChangedDg.Broadcast(Data.NewValue);
}

void URootHUDViewModel::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	OnMaxManaChangedDg.Broadcast(Data.NewValue);
}

void URootHUDViewModel::OnAssetTagsApplied(const FGameplayTagContainer& TagContainer)
{
	if (!MessageDataTable)
		return;
	
	for (const FGameplayTag& Tag : TagContainer)
	{
		if (const FName* RowNamePtr = MessageDataTableTagMap.Find(Tag))
		{
			if (const FTagUIWidgetRow* Row = MessageDataTable->FindRow<FTagUIWidgetRow>(*RowNamePtr, TEXT("TagUIWidgetRow")))
			{
				OnTagMessageInvokedDg.Broadcast(*Row);
			}
		}
	}
}

void URootHUDViewModel::OnCharacterApplyDamage(AActor* Target, float DamageValue, bool bIsBlockedHit, bool bIsCriticalHit)
{
	if (!Target || !Params.PlayerControllerCache)
		return;
	
	FVector2D ResultPos = FVector2D::ZeroVector;
	if (UGameplayStatics::ProjectWorldToScreen(
		Params.PlayerControllerCache,
		Target->GetActorLocation(),
		ResultPos))
	{
		OnShowDamageTextDg.Broadcast(Target->GetActorLocation(), DamageValue, bIsBlockedHit, bIsCriticalHit);
	}
}

void URootHUDViewModel::OnXPChanged(int32 OldXP, int32 NewXP)
{
	OnXPPercentChangedDg.Broadcast(GetXPPercent(NewXP));
}

void URootHUDViewModel::OnPlayerLevelChanged(int32 OldLevel, int32 NewLevel)
{
	OnPlayerLevelUpgradeDg.Broadcast(false, OldLevel, NewLevel);
}

float URootHUDViewModel::GetXPPercent(int32 InXP) const
{
	if (const ARPGPlayerState* PS = Cast<ARPGPlayerState>(Params.PlayerStateCache))
	{
		if (ULevelUpInfoDataAsset* LevelUpInfo = PS->GetLevelUpInfo())
		{
			const int32 Level = LevelUpInfo->GetLevelForXP(InXP);
			
			// 目标等级无效，直接返回0
			if (Level < 0)
				return 0.f;
			// 当前等级为0级
			if (Level - 1 < 0)
				return static_cast<float>(InXP) / LevelUpInfo->LevelUpInfos[Level].LevelUpRequirement;
			// 已到达最大等级
			if (Level >= LevelUpInfo->LevelUpInfos.Num())
				return 1.f;
			
			const int32 CurLevelXP = LevelUpInfo->LevelUpInfos[Level - 1].LevelUpRequirement;
			const int32 NextLevelXP = LevelUpInfo->LevelUpInfos[Level].LevelUpRequirement;
			return static_cast<float>(InXP - CurLevelXP) / (NextLevelXP - CurLevelXP);
		}
	}
	return 0.f;
}
