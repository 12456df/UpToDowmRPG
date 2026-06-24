

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "CharacterBase.generated.h"

class USkeletalMeshComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
UCLASS(Abstract)
class AURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const{return AttributeSet;};
	virtual void InitAbilityActorInfo();
	
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;
	/*Combat Interface*/
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	/*end Combat Interface*/

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	

	/*
	*使用GameplayEffect给角色初始化属性
	*/
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const;
	virtual void InitializeDefaultAttributes() const;
	
	void AddCharacterAbilities();

	/* Dissolve Effects */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	bool bDead = false;
private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
