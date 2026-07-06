


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"
#include "Player/AuraPlayerState.h"


void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayAttribute& Attribute, const FGameplayTag& AttributeTag) const{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BroadcastInitialValues(){
	Super::BroadcastInitialValues();

	check(AttributeInfo);

	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Value(), Pair.Key);
	}

	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
   
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies(){
	Super::BindCallbacksToDependencies();

	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda
		([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Value(), Pair.Key);
		});
	}

	GetAuraPS()->OnAttributePointsChanged.AddUObject(this, &UAttributeMenuWidgetController::OnAttributePointsChanged);

}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraAuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::OnAttributePointsChanged(int32 AttributePoints)
{
	AttributePointsChangedDelegate.Broadcast(AttributePoints);
}