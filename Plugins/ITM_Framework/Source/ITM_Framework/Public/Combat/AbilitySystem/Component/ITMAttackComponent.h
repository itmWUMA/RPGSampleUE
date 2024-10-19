// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Combat/AbilitySystem/ITMAbilityDefines.h"
#include "Components/ActorComponent.h"
#include "ITMAttackComponent.generated.h"


class UITMAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), DisplayName = "Attack Component", meta=(BlueprintSpawnableComponent) )
class ITM_FRAMEWORK_API UITMAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void Init(UITMAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle& AbilitySpecHandle);
	void Attack(AActor* Target, const FAttackData& InAttackData);
	
	void SetCurrentHitResult(const FHitResult& HitResult) { CurrentHitResult = HitResult; }
	const FHitResult& GetCurrentHitResult() const { return CurrentHitResult; }
	void ClearHitResult() { CurrentHitResult.Init(); }

protected:
	virtual void BeginPlay() override;
	virtual void MakeAttackContext(const FAttackData& InAttackData, OUT FGameplayEffectContextHandle& ContextHandle);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS|Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	FHitResult CurrentHitResult;
	
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	TWeakObjectPtr<UITMAbilitySystemComponent> ASCCache = nullptr;
	FGameplayAbilitySpecHandle AttackAbilitySpecHandle;
};
