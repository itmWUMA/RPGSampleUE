// Copyright 2024 Itmwuma


#include "Settings/ITMFrameworkSettings.h"

const UITMFrameworkSettings* UITMFrameworkSettings::Get()
{
	return GetDefault<UITMFrameworkSettings>();
}
