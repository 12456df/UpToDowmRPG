


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"
#include "Aura/AuraLogChannels.h"
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if(!bStartupAbilitiesGiven)
	{
        bStartupAbilitiesGiven = true;
		AbilityGivenDelegate.Broadcast(this);
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	if (GetOwnerRole() == ROLE_Authority)
	{
		ClientEffectApplied(TagContainer, EffectSpec, ActiveEffectHandle);
	}
	else
	{
		EffectAssetTags.Broadcast(TagContainer);
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(const FGameplayTagContainer& AssetTags,const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
    {
        FGameplayAbilitySpec AbilitySpec  = FGameplayAbilitySpec(AbilityClass, 1);
        if(const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
        {
            AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupGameplayTag);
            GiveAbility(AbilitySpec);
        }
    }
    bStartupAbilitiesGiven = true;
    AbilityGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if(!InputTag.IsValid()) return;
    for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if(AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
        {
           AbilitySpecInputPressed(AbilitySpec);
           if(!AbilitySpec.IsActive())
           {
            TryActivateAbility(AbilitySpec.Handle);
           }
        }
    }
}
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if(!InputTag.IsValid()) return;
    for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if(AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
        {
            AbilitySpecInputReleased(AbilitySpec);
        }
    }
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for(const FGameplayAbilitySpec& AbilitySpec :GetActivatableAbilities())
    {
        if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
    }
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        for(const FGameplayTag& Tag : AbilitySpec.Ability->AbilityTags)
        {
            if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        for(const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
        {
            if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}