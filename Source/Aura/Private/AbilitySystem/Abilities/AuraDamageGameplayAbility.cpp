


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
    FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.0f);
    for(const TTuple<FGameplayTag, FScalableFloat>& DamageType : DamageTypes)
    {
        const float FloatValue = DamageType.Value.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType.Key, FloatValue);
        GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
    }
}


FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}