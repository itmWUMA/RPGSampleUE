// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Load/SaveActor.h"
#include "LevelNode.generated.h"

UCLASS()
class RPGDEMO_API ALevelNode : public AActor, public ISaveActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Execute();

	// --- ISaveActor Impl ---
	virtual bool ShouldLoadTransform_Implementation() override { return false;}
	virtual void OnActorLoaded_Implementation() override;
	// --- ISaveActor Impl End ---

protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Execute")
	void BP_OnExecute();
	virtual void OnExecute();

private:
	void ExecuteNextNode() const;
	void LoadLevelNode();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ALevelNode> LinkNode;

	UPROPERTY(EditAnywhere, Category = "LevelNode|Save", meta = (ToolTip="是否自定义SaveExecute标记\n默认在Execute时进行标记"))
	bool bCustomMarkExecuted = false;

	UPROPERTY(EditAnywhere, Category = "LevelNode|Save", meta = (ToolTip="Load时是否再执行Execute逻辑"))
	bool bShouldExecuteWhenLoad = false;

	UPROPERTY(EditAnywhere, Category = "LevelNode|Save", meta = (ToolTip="是否为LevelNode链的起始加载点"))
	bool bLoadStart = false;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bSaveGameExecuted = false;
};
