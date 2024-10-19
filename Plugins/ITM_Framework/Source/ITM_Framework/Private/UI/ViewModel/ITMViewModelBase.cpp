// Copyright 2024 Itmwuma


#include "UI/ViewModel/ITMViewModelBase.h"

FITMViewModelParams& FITMViewModelParams::operator=(const FITMViewModelParams& InParams)
{
	if (this == &InParams)
		return *this;

	PlayerControllerCache = InParams.PlayerControllerCache;
	PlayerStateCache = InParams.PlayerStateCache;
	ASCCache = InParams.ASCCache;
	AttributeSetCache = InParams.AttributeSetCache;

	return *this;
}

void UITMViewModelBase::OnInit()
{
	BindCallbackToDependencies();
	BroadcastInitialValues();
}

void UITMViewModelBase::OnEnd()
{
	UnbindCallbackToDependencies();
}

void UITMViewModelBase::SetViewModelParams(const FITMViewModelParams& InParams)
{
	Params = InParams;
}
