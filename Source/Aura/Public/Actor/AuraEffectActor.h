
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"


class UGameplayEffect;
class UAbilitySystemComponent;	
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
};
UCLASS()
class AURA_API AAuraEffectActor : public AActor

{
	GENERATED_BODY()
public:	
	AAuraEffectActor();
protected:

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
    bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	EEffectRemovePolicy InfiniteEffectRemovePolicy = EEffectRemovePolicy::DoNotRemove;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
	float ActorLevel = 1.f;

	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;
};



