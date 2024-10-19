// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Core/ITMPlayerController.h"
#include "RPGPlayerController.generated.h"

class UDitherComponent;
class IHighlightable;
class UInteractionManager;
struct FInputActionValue;
class UInputAction;

/**
 * 
 */
UCLASS()
class RPGDEMO_API ARPGPlayerController : public AITMPlayerController
{
	GENERATED_BODY()

public:
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "GAS|Input")
	bool IsSkillModifyTriggered() const { return bSkillModifyTriggered; }
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void OnMoveActionTriggered(const FInputActionValue& InputActionValue);
	void OnOpenAttributeMenuActionTriggered(const FInputActionValue& InputActionValue);
	void OnSkillModifyInputCallback(const FInputActionValue& InputActionValue, bool bTriggered);
	void OnOpenSaveMenuTriggered(const FInputActionValue& InputActionValue);

	bool CheckActorHighlightEnable(const AActor* InActor) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> OpenAttributeMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> SkillModifyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> OpenSaveMenuAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float HighlightEnableDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Dither")
	TEnumAsByte<ECollisionChannel> DitherTraceChannel = ECC_Visibility;

private:
	TWeakObjectPtr<UInteractionManager> InteractionMgrCache = nullptr;
	
	UPROPERTY(Transient)
	TArray<AActor*> HighlightedObjectsCache;

	bool bSkillModifyTriggered = false;

	TSet<TWeakObjectPtr<UDitherComponent>> CurrentDitherComps;
};
