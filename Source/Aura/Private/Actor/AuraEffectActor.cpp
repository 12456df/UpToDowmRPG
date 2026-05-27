
#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SceneComponent.h"
#include "GameplayEffect.h"

AAuraEffectActor::AAuraEffectActor()

{

	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()

{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> GameplayEffectClass){

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr)return;
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel,EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	bool bIsInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap){
		ActiveEffectHandles.Add(ActiveEffectHandle,TargetASC);
	}
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor){
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap){
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
	

}
void AAuraEffectActor::OnEndOverlap(AActor* TargetActor){
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap){
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap){
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	//TODO:add array to DurationEffect

	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap){
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
	if(InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap){
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(TargetASC))return;
		
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for(auto& ActiveEffectHandle : ActiveEffectHandles){
			if(ActiveEffectHandle.Value == TargetASC){
				TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle.Key);
				HandlesToRemove.Add(ActiveEffectHandle.Key);
			}
		}
		for(auto& Handle : HandlesToRemove){
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
