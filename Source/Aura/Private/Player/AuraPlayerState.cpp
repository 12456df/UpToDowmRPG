


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Aura/AuraLogChannels.h"
AAuraPlayerState::AAuraPlayerState()
{
	SetNetUpdateFrequency(100.f);
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

}
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const{
    return AbilitySystemComponent;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AAuraPlayerState, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(AAuraPlayerState, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(AAuraPlayerState, AttributePoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(AAuraPlayerState, SpellPoints, COND_None, REPNOTIFY_Always);
}
void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChanged.Broadcast(Level);
}
void AAuraPlayerState::SetLevel(int32 NewLevel)
{
	if(!HasAuthority())return;
	Level = NewLevel;
	OnLevelChanged.Broadcast(Level);
}
int32 AAuraPlayerState::AddToLevel(int32 LevelToAdd)
{
	if(!HasAuthority())return 0;
	Level += LevelToAdd;
	OnLevelChanged.Broadcast(Level);
	
	return Level;
}

void AAuraPlayerState::SetXP(int32 NewXP)
{
	if(!HasAuthority())return;
	XP = NewXP;
	OnXPChanged.Broadcast(XP);
}
int32 AAuraPlayerState::AddToXP(int32 XPToAdd)
{
	if(!HasAuthority())return 0;
	XP += XPToAdd;
	UE_LOG(LogAura, Log, TEXT("XP on %s: %d"), *GetName(), XP);
	OnXPChanged.Broadcast(XP);
	return XP;
}
void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChanged.Broadcast(XP);
}

void AAuraPlayerState::SetAttributePoints(int32 NewAttributePoints)
{
	if(!HasAuthority())return;
	AttributePoints = NewAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
int32 AAuraPlayerState::AddToAttributePoints(int32 AttributePointsToAdd)
{
	if(!HasAuthority())return 0;
	AttributePoints += AttributePointsToAdd;
	OnAttributePointsChanged.Broadcast(AttributePoints);
	return AttributePoints;
}
void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}


void AAuraPlayerState::SetSpellPoints(int32 NewSpellPoints)
{
	if(!HasAuthority())return;
	SpellPoints = NewSpellPoints;
	OnSpellPointsChanged.Broadcast(SpellPoints);
}
int32 AAuraPlayerState::AddToSpellPoints(int32 SpellPointsToAdd)
{
	if(!HasAuthority())return 0;
	SpellPoints += SpellPointsToAdd;
	OnSpellPointsChanged.Broadcast(SpellPoints);
	return SpellPoints;
}
void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChanged.Broadcast(SpellPoints);
}
