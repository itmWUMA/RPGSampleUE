// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterJobInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterJob : uint8
{
	None,
	Warrior,
	Ranger,
	Magician,
};

USTRUCT()
struct FCharacterJobDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributesEffect;
};

/**
 * 
 */
UCLASS()
class RPGDEMO_API UCharacterJobInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	const FCharacterJobDefaultInfo& GetCharacterJobDefaultInfo(ECharacterJob Job) const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Common")
	TSubclassOf<UGameplayEffect> VitalAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	TSubclassOf<UGameplayEffect> AttributeCalculationEffect;

	UPROPERTY(EditDefaultsOnly)
	TMap<ECharacterJob, FCharacterJobDefaultInfo> CharacterJobInfo;
};
