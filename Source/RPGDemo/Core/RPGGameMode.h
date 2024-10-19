// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/ITMUserInterfaceDefines.h"
#include "RPGGameMode.generated.h"

class UCharacterJobInfo;
class UAttributeMenu;

/**
 * 
 */
UCLASS()
class RPGDEMO_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	FORCEINLINE const FITMWidgetClassConfig& GetRootHUDClass() const { return RootHUDClass; }
	FORCEINLINE const FITMWidgetClassConfig& GetAttributeMenuClass() const { return AttributeMenuClass; }
	FORCEINLINE const UCharacterJobInfo* GetCharacterJobInfo() const { return CharacterJobInfo; }
	FORCEINLINE const UCurveTable* GetEffectCoefficients() const { return EffectCoefficients; }

	void ShowAttributeMenu();
	void CloseAttributeMenu();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	FITMWidgetClassConfig RootHUDClass;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	FITMWidgetClassConfig AttributeMenuClass;

	UPROPERTY(EditAnywhere, Category = "Character")
	TObjectPtr<UCharacterJobInfo> CharacterJobInfo;

	UPROPERTY(EditAnywhere, Category = "Character")
	TObjectPtr<UCurveTable> EffectCoefficients;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAttributeMenu> AttributeMenuWidget;
};
