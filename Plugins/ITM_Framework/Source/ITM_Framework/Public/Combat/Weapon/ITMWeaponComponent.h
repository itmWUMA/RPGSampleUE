// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ITMWeaponComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponDetached, USkeletalMeshComponent* WeaponMeshComp);

UCLASS( ClassGroup=(Custom), DisplayName = "ITM Weapon Component", meta=(BlueprintSpawnableComponent) )
class ITM_FRAMEWORK_API UITMWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UITMWeaponComponent();
	virtual void OnComponentCreated() override;
	
	UFUNCTION(BlueprintPure, Category = "Combat|Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMeshComp; }

	virtual void DetachWeapon();
	FORCEINLINE FOnWeaponDetached& GetOnWeaponDetachedDelegate() { return OnWeaponDetachedDelegate; }
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName WeaponSocket = TEXT("WeaponSocket");

private:
	FOnWeaponDetached OnWeaponDetachedDelegate;
	FTimerHandle DestroyTimerHandle;
};
