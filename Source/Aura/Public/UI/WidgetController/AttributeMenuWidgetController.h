

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
class UAttributeInfo;//DataAsset
struct FAuraAttributeInfo;
struct FGameplayAttribute;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate, const FAuraAttributeInfo&, AttributeInfo);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UPROPERTY(BlueprintAssignable)
	FAttributeInfoDelegate AttributeInfoDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
	
private:

	void BroadcastAttributeInfo(const FGameplayAttribute& Attribute, const FGameplayTag& AttributeTag) const;
	void OnAttributePointsChanged(int32 AttributePoints);
};
