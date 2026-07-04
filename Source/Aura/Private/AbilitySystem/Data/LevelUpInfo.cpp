


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
    // 从 1 开始，跳过占位符
    for (int32 i = 1; i < LevelUpInformation.Num(); ++i)
    {
        if (XP < LevelUpInformation[i].LevelUpRequirement)
        {
            return i;  
        }
    }
    return LevelUpInformation.Num() > 0 ? LevelUpInformation.Num() : 1;
}