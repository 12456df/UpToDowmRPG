


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    for (const FAuraInputAction& Action : AbilityInputActions)
    {
        if (Action.InputTag == InputTag)
        {
            return Action.InputAction;
        }
    }
    if (bLogNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("Input Action with tag %s not found"), *InputTag.ToString());
    }
    return nullptr;
}