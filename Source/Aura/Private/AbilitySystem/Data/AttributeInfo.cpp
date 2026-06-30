


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
    for (const FAuraAttributeInfo& Info : AttributeInformation)
    {
        if (Info.AttributeTag.MatchesTag(AttributeTag))
        {
            return Info;
        }
    }
    if (bLogNotFound)
    {
        UE_LOG(LogAura, Error, TEXT("AttributeInfo: Attribute tag %s not found"), *AttributeTag.ToString());
    }
    return FAuraAttributeInfo();
}
