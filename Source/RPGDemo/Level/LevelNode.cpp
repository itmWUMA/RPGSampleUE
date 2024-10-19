// Copyright 2024 Itmwuma


#include "Level/LevelNode.h"

void ALevelNode::Execute()
{
	if (bSaveGameExecuted)
	{
		LoadLevelNode();
		return;
	}
	
	OnExecute();
	ExecuteNextNode();
}

void ALevelNode::OnActorLoaded_Implementation()
{
	if (bSaveGameExecuted && bLoadStart)
		LoadLevelNode();
}

void ALevelNode::OnExecute()
{
	if (!bCustomMarkExecuted)
		bSaveGameExecuted = true;
	BP_OnExecute();
}

void ALevelNode::ExecuteNextNode() const
{
	if (IsValid(LinkNode))
		LinkNode->Execute();
}

void ALevelNode::LoadLevelNode()
{
	if (bShouldExecuteWhenLoad)
		OnExecute();
	ExecuteNextNode();
}
