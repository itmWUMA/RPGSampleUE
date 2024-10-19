// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/AbilitySystem/ITMAbilityDefines.h"
#include "Components/ActorComponent.h"
#include "ITMHurtComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHurtStatusChanged, bool bHurt);


class UAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), DisplayName = "Hurt Component", meta=(BlueprintSpawnableComponent) )
class ITM_FRAMEWORK_API UITMHurtComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GAS|Damage")
	bool IsHurting() const { return bHurting; }

	UFUNCTION(BlueprintPure, Category = "GAS|Damage")
	const FHurtDataCollection& GetCurrentHurtData() const { return CurrentHurtData; }

	UFUNCTION(BlueprintCallable, Category = "GAS|Damage")
	void SetCurrentHurtData(const FHurtDataCollection& InCurrentHurtData) { CurrentHurtData = InCurrentHurtData; }

	FORCEINLINE FOnHurtStatusChanged& GetOnHurtDelegate() { return OnHurtDelegate; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnHurtTagChanged(const FGameplayTag HurtTag, int32 NewCount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHurting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FHurtDataCollection CurrentHurtData; 
	
private:
	FOnHurtStatusChanged OnHurtDelegate;
	
	TWeakObjectPtr<UAbilitySystemComponent> ASCCache = nullptr;
	FDelegateHandle OnHurtTagChangedHandle;
};
