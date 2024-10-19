// Copyright 2024 Itmwuma


#include "Character/CharacterJobInfo.h"
 
const FCharacterJobDefaultInfo& UCharacterJobInfo::GetCharacterJobDefaultInfo(ECharacterJob Job) const
{
	return CharacterJobInfo.FindChecked(Job);
}
