#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"

#include "Enemy.generated.h"


class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AEnemy : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemy();
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	/**enemy Interface*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/**end enemy Interface*/

	/*
	*Combat Interface
	*/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;

	virtual void SetCombatTarget_Implementation(AActor* inCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	/*
	*End Combat Interface
	*/
	UFUNCTION()
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedDelegateSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedDelegateSignature OnMaxHealthChanged;


protected:
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, Category = "Highlighting")
	TObjectPtr<UMaterialInterface> DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Highlighting")
	TObjectPtr<UMaterialInterface> HighlightedMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Highlighting")
	bool bHighlighted = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};