


#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
	
}
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const{
    return AbilitySystemComponent;
}