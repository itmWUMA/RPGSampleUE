// Copyright 2024 Itmwuma

#include "ITM_Framework.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "UI/GameplayDebugger_UI.h"
#endif

#define LOCTEXT_NAMESPACE "FITM_FrameworkModule"

void FITM_FrameworkModule::StartupModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("UI", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebugger_UI::MakeInstance), EGameplayDebuggerCategoryState::Disabled, 7);
#endif
}

void FITM_FrameworkModule::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.UnregisterCategory("UI");
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FITM_FrameworkModule, ITM_Framework)