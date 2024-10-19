// Copyright 2024 Itmwuma

#if WITH_GAMEPLAY_DEBUGGER

#include "UI/GameplayDebugger_UI.h"

FGameplayDebugger_UI::FGameplayDebugger_UI()
{
	SetDataPackReplication<FRepData>(&DataPack);
}

void FGameplayDebugger_UI::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	const TSharedPtr<SWidget> FocusWidget = FSlateApplication::Get().GetUserFocusedWidget(0);
	DataPack.FocusWidget = FReflectionMetaData::GetWidgetDebugInfo(FocusWidget.Get());
}

void FGameplayDebugger_UI::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Printf(TEXT("[Focus Widget]:{yellow}%s"), *DataPack.FocusWidget);
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebugger_UI::MakeInstance()
{
	return MakeShareable(new FGameplayDebugger_UI());
}

void FGameplayDebugger_UI::FRepData::Serialize(FArchive& Ar)
{
}

#endif