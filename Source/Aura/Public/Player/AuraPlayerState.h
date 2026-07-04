

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatDelegate, int32);
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStatDelegate OnXPChanged;
	FOnPlayerStatDelegate OnLevelChanged;
	FOnPlayerStatDelegate OnAttributePointsChanged;
	FOnPlayerStatDelegate OnSpellPointsChanged;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const{return AttributeSet;};

	FORCEINLINE int32 GetPlayerLevel() const{return Level;};
	void SetLevel(int32 NewLevel);
	int32 AddToLevel(int32 LevelToAdd);

	FORCEINLINE int32 GetXP() const{return XP;};
	void SetXP(int32 NewXP);
	int32 AddToXP(int32 XPToAdd);
	
	FORCEINLINE int32 GetAttributePoints() const{return AttributePoints;};
	void SetAttributePoints(int32 NewAttributePoints);
	int32 AddToAttributePoints(int32 AttributePointsToAdd);

	FORCEINLINE int32 GetSpellPoints() const{return SpellPoints;};
	void SetSpellPoints(int32 NewSpellPoints);
	int32 AddToSpellPoints(int32 SpellPointsToAdd);
private:

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
};
