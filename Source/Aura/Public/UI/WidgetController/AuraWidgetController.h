

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

class UAbilitySystemComponent;
class UAttributeSet;
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS){}

	UPROPERTY(EditAnywhere, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(EditAnywhere, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(EditAnywhere, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditAnywhere, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;
};

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;

};
