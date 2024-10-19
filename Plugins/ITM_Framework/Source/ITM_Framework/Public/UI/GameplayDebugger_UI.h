// Copyright 2024 Itmwuma

#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

/**
 * 
 */
class ITM_FRAMEWORK_API FGameplayDebugger_UI : public FGameplayDebuggerCategory
{
public:
	FGameplayDebugger_UI();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	struct FRepData
	{
		FString FocusWidget;
		
		void Serialize(FArchive& Ar);
	};
	FRepData DataPack;
};

#endif