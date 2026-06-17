


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
    UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
    return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
   const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
   if(bIsLocallyControlled)
   {
        SendMouseCursorData();
   }
   else
   {
        const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
        const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
        /*等待 ServerSetReplicatedTargetData 的回调
            AbilityTargetDataSetDelegate():根据「哪条技能 + 哪次激活」，返回对应的那一份委托。
        */
        AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
        const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
        if (!bCalledDelegate)
        {
            SetWaitingOnRemotePlayerData();
        }
   }
}

void UTargetDataUnderMouse::SendMouseCursorData()
{

    FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

    APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
    FHitResult HitResult;
    PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
    

    FGameplayAbilityTargetDataHandle DataHandle;
    FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
    Data->HitResult = HitResult;
    DataHandle.Add(Data);
    /*一种广播*/
    AbilitySystemComponent->ServerSetReplicatedTargetData(
        GetAbilitySpecHandle(), 
        GetActivationPredictionKey(), 
        DataHandle, 
        FGameplayTag(), 
        AbilitySystemComponent->ScopedPredictionKey);

    if(ShouldBroadcastAbilityTaskDelegates())
    {
        ValidData.Broadcast(DataHandle);
    }
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
    AbilitySystemComponent->ConsumeClientReplicatedTargetData(
        GetAbilitySpecHandle(), GetActivationPredictionKey());
    if(ShouldBroadcastAbilityTaskDelegates())
    {
        /*使server上的蓝图能收到数据*/
        ValidData.Broadcast(DataHandle);
    }
}