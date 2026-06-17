

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"


class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AEnemy : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemy();
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	/**enemy Interface*/
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	/**end enemy Interface*/

	/*
	*Combat Interface
	*/
	virtual int32 GetPlayerLevel() override;
	/*
	*End Combat Interface
	*/

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedDelegateSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedDelegateSignature OnMaxHealthChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Highlighting")
	TObjectPtr<UMaterialInterface> DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Highlighting")
	TObjectPtr<UMaterialInterface> HighlightedMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Highlighting")
	bool bHighlighted = false;
protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
