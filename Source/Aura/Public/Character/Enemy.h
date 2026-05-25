

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemy : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemy();

	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Highlighting")
	TObjectPtr<UMaterialInterface> DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Highlighting")
	TObjectPtr<UMaterialInterface> HighlightedMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Highlighting")
	bool bHighlighted = false;
};
