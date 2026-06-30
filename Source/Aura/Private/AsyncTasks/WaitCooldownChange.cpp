


#include "AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
    //创建一个WaitCooldownChange对象，同时初始化ASC和CooldownTag
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;
	
    //创建失败
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	//向ASC注册事件，当CooldownTag被添加或移除时，调用CooldownTagChanged函数

    /*  
        DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameplayEffectTagCountChanged, FGameplayTag, int32);
        FOnGameplayEffectTagCountChanged& RegisterGameplayTagEvent(
        FGameplayTag Tag,
        EGameplayTagEventType EventType = EGameplayTagEventType::NewOrRemoved);
    *   如上所知，RegisterGameplayTagEvent返回的是一个委托，包含两个参数：FGameplayTag和int32
    */
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChanged);

	//OnActiveGameplayEffectAddedDelegateToSelf 「有一个 Gameplay Effect 被施加到我（这个 ASC）身上时」会广播的委托。
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
    /*RegisterGameplayTagEvent(CooldownTag, NewOrRemoved)   拿到「这个 Cooldown Tag 变化」的委托
    * RemoveAll(this)                                       移除所有这个委托的回调
    */
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
        //只广播Tag被移除时，冷却时间结束
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
        //「请找出我身上所有正在授予 Cooldown.Fire.Firebolt 的 Active Gameplay Effect。」
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
        //「找出所有正在授予 Cooldown.Fire.Firebolt 的 Active Gameplay Effect 的剩余时间」
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{
			float TimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}
			//「这个冷却时间剩余多少秒？」
            //只广播Tag被添加时，冷却时间开始
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}

/*
AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChanged);

          
两步各自干什么
① RegisterGameplayTagEvent(InCooldownTag, NewOrRemoved)
作用：向 ASC 登记「我要听哪个 Tag、听哪种变化」，并拿到对应的委托引用。

听谁：InCooldownTag（如 Cooldown.Fire.Firebolt）
听什么：NewOrRemoved = Tag 刚加上 或 刚移除 时才算一次事件
返回值：ASC 内部维护的 多播委托（还没你的回调，只是「订阅入口」）
这一步定的是：什么情况下算「冷却相关事件发生了」（时机 / 条件）。

② .AddUObject(WaitCooldownChange, &CooldownTagChanged)
作用：在①的委托上挂上你的处理函数。

对谁调：WaitCooldownChange（这个 Async Task 实例）
调什么：CooldownTagChanged
参数谁填：ASC 在 Tag 变化时 Broadcast(Tag, NewCount)，不是你传的
这一步定的是：事件发生后，由谁、用什么函数来响应。

*/