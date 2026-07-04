


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
//初始化
void UOverlayWidgetController::BroadcastInitialValues(){
	Super::BroadcastInitialValues();
    const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());  
}

//检测属性变化
void UOverlayWidgetController::BindCallbacksToDependencies(){
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

    /**
    <line34-39>详细解释

    1、这里包含了两个委托
    2、第一个委托的委托实例引用：AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
       回调函数：AddLambda（）
    3、第二个委托的委托实例： OnHealthChanged
       广播：OnHealthChanged.Broadcast(Data.NewValue)
    总结：第一个委托（GAS Health 变化）的回调 Lambda 里，调用了第二个委托实例 OnHealthChanged 的 Broadcast。
    */

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
       [this](const FOnAttributeChangeData& Data)
       {
            OnHealthChanged.Broadcast(Data.NewValue);
       }
    );

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxHealthChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnManaChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxManaChanged.Broadcast(Data.NewValue);
        }
    );

    AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
    AuraPlayerState->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

    AuraPlayerState->OnLevelChanged.AddLambda(
        [this](int32 NewLevel)
        {
            OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
        }
    );

    if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        if(AuraASC->bStartupAbilitiesGiven)
        {
            OnInitializeStartupAbilities(AuraASC);
        }
        else
        {
            AuraASC->AbilityGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
        }

        AuraASC->EffectAssetTags.AddLambda(
            [this](const FGameplayTagContainer& AssetTags)
            {
                for(const FGameplayTag& Tag : AssetTags)
                {
                    const FString Msg = FString::Printf(TEXT("Asset Tag:%s"),*Tag.ToString());
                    GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Green,Msg);
    
                    FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(TEXT("Message"));
                    if(Tag.MatchesTag(MessageTag))
                    {
                        FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
                        MessageWidgetRowDelegate.Broadcast(*Row);
                    }
                }
            }
        );
    }
    
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	//TODO Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;
    FForEachAbility ForEachAbilityDelegate;
    ForEachAbilityDelegate.BindLambda(
        [this,AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
        {
            FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
            Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
            AbilityInfoDelegate.Broadcast(Info);

        }
    );
    AuraAbilitySystemComponent->ForEachAbility(ForEachAbilityDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
    AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
    ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
    if(!LevelUpInfo) return;

    int32 CurrentLevel = FMath::Clamp(LevelUpInfo->FindLevelForXP(NewXP), 1, LevelUpInfo->LevelUpInformation.Num() - 1);

    int32 CurrentLevelXP = LevelUpInfo->LevelUpInformation[CurrentLevel].LevelUpRequirement - LevelUpInfo->LevelUpInformation[CurrentLevel - 1].LevelUpRequirement;
    if(CurrentLevelXP <= 0) return;
        
    int32 XPForCurrentLevel = NewXP - LevelUpInfo->LevelUpInformation[CurrentLevel - 1].LevelUpRequirement;

    OnXPPercentChangedDelegate.Broadcast(FMath::Clamp(static_cast<float>(XPForCurrentLevel) / static_cast<float>(CurrentLevelXP), 0.f, 1.f));
}