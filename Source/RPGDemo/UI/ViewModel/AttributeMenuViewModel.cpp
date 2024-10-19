// Copyright 2024 Itmwuma


#include "UI/ViewModel/AttributeMenuViewModel.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Core/RPGPlayerState.h"

bool UAttributeInfoDataAsset::GetAttributeInfo(const FGameplayAttribute& Attribute, FAttributeShowInfo& OutInfo) const
{
	for (const FAttributeShowInfo& Info : ListenAttributeInfo)
	{
		if (Info.Attribute == Attribute)
		{
			OutInfo = Info;
			return true;
		}
	}

	UE_LOG(LogAbilitySystemComponent, Error, TEXT("Cannot find Attribute[%s] in Attribute info"), *Attribute.AttributeName);
	return false;
}

void UAttributeMenuViewModel::BroadcastInitialValues()
{
	if (!AttributeShowInfo || !Params.AttributeSetCache || !Params.PlayerStateCache)
		return;
		
	 for (const FAttributeShowInfo& ShowInfo : AttributeShowInfo->ListenAttributeInfo)
	 {
	 	OnListenedAttributeChangedDg.Broadcast(ShowInfo, ShowInfo.Attribute.GetNumericValue(Params.AttributeSetCache));
	 }

	if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(Params.PlayerStateCache))
	{
		OnAttributePointChangedDg.Broadcast(PS->GetAttributePoint());
	}
}

void UAttributeMenuViewModel::BindCallbackToDependencies()
{
	if (!AttributeShowInfo || !Params.ASCCache || !Params.PlayerStateCache)
		return;

	for (const FAttributeShowInfo& ShowInfo : AttributeShowInfo->ListenAttributeInfo)
	{
		FDelegateHandle Handle = Params.ASCCache->GetGameplayAttributeValueChangeDelegate(ShowInfo.Attribute)
			.AddUObject(this, &UAttributeMenuViewModel::OnListenedAttributeChanged);
		ListenedAttributeDelegateHandle.Add(ShowInfo.Attribute, Handle);
	}

	if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(Params.PlayerStateCache))
	{
		PS->GetOnAttributePointChanged().AddUObject(this, &UAttributeMenuViewModel::OnAttributeChanged);
	}
}

void UAttributeMenuViewModel::UnbindCallbackToDependencies()
{
	if (!AttributeShowInfo || !Params.ASCCache)
		return;

	for (const FAttributeShowInfo& ShowInfo : AttributeShowInfo->ListenAttributeInfo)
	{
		if (FDelegateHandle* HandlePtr = ListenedAttributeDelegateHandle.Find(ShowInfo.Attribute))
		{
			Params.ASCCache->GetGameplayAttributeValueChangeDelegate(ShowInfo.Attribute)
				.Remove(*HandlePtr);
		}
	}

	if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(Params.PlayerStateCache))
	{
		PS->GetOnAttributePointChanged().RemoveAll(this);
	}

	ListenedAttributeDelegateHandle.Empty();
}

void UAttributeMenuViewModel::UpgradeAttribute(const FGameplayAttribute& InAttribute)
{
	if (URPGAbilitySystemComponent* ASC = Cast<URPGAbilitySystemComponent>(Params.ASCCache))
	{
		ASC->UpgradeAttribute(InAttribute);
	}
}

void UAttributeMenuViewModel::OnListenedAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!AttributeShowInfo)
		return;
	
	FAttributeShowInfo ShowInfo;
	if (AttributeShowInfo->GetAttributeInfo(Data.Attribute, ShowInfo))
		OnListenedAttributeChangedDg.Broadcast(ShowInfo, Data.NewValue);
}

void UAttributeMenuViewModel::OnAttributeChanged(int32 OldAttributePoint, int32 NewAttributePoint)
{
	OnAttributePointChangedDg.Broadcast(NewAttributePoint);
}
