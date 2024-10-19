// Copyright 2024 Itmwuma


#include "AbilitySystem/XP/LevelUpInfoDataAsset.h"

int32 ULevelUpInfoDataAsset::GetLevelForXP(int32 InXP) const
{
	if (LevelUpInfos.IsEmpty())
		return -1;
	
	int32 Left = 0, Right = LevelUpInfos.Num(), Mid = -1;
	while (Left < Right)
	{
		Mid = Left + ((Right - Left) >> 1);
		if (LevelUpInfos[Mid].LevelUpRequirement > InXP)
			Right = Mid;
		else if (LevelUpInfos[Mid].LevelUpRequirement < InXP)
			Left = Mid + 1;
		else if (LevelUpInfos[Mid].LevelUpRequirement == InXP)
			Left = Mid + 1;
	}

	return Left;
}

int32 ULevelUpInfoDataAsset::GetAttributePointReward(int32 InLevel) const
{
	return LevelUpInfos.IsValidIndex(InLevel) ? LevelUpInfos[InLevel].AttributePointReward : 0;
}
