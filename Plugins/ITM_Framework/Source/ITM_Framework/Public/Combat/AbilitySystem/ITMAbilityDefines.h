// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "NiagaraSystem.h"
#include "GameplayEffectTypes.h"
#include "ITMAbilityDefines.generated.h"

// Attributes

#ifndef ATTRIBUTE_ACCESSORS
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
#endif

#define PRE_ATTRIBUTE_CLAMP(AttributeName, AttributeArgName, NewValueArgName, MinValue) \
	if (AttributeArgName == Get##AttributeName##Attribute()) \
	{ \
		NewValueArgName = FMath::Clamp(NewValueArgName, MinValue, GetMax##AttributeName()); \
	}

#define POST_ATTRIBUTE_CLAMP(AttributeName, EffectModCallbackDataArgName, MinValue) \
if (EffectModCallbackDataArgName.EvaluatedData.Attribute == Get##AttributeName##Attribute()) \
	{ \
		Set##AttributeName(FMath::Clamp(Get##AttributeName(), MinValue, GetMax##AttributeName())); \
	}

// ASC

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssetTagsApplied, const FGameplayTagContainer&);

// Effect

USTRUCT(BlueprintType)
struct FGameplayEffectActorData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> AvatarActor;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AController> Controller;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacter> Character;
};

USTRUCT(BlueprintType)
struct FGameplayEffectData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectActorData SourceData;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectActorData TargetData;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectContextHandle ContextHandle;
};

// Others

USTRUCT(BlueprintType)
struct FArtisticEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> ParticleAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> SoundAsset;
};

USTRUCT(BlueprintType)
struct FHurtEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "AnimAsset==nullptr", ClampMin = 0.f, UIMin = 0.f))
	float EffectDelayTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FArtisticEffectData AdditionalEffectData;
};

USTRUCT(BlueprintType)
struct FHurtDataCollection
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag HurtTypeTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FHurtEffectDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FHurtEffectData> HurtEffectData;
};

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat DamageValue = FScalableFloat(1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag PendingHurtType = FGameplayTag::EmptyTag;

	float GetDamageValue(int32 InLevel) const;
};

USTRUCT()
struct ITM_FRAMEWORK_API FDamageGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FAttackData AttackData;
	
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FGameplayEffectContext* Duplicate() const override;
};