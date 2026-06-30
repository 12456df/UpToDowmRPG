

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
 UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();
protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};


/*
继承 UBlueprintAsyncActionBase 是因为 WaitCooldownChange 要做成 蓝图里的「异步节点」：
调用一次后 不立刻结束，而是 挂着听 GAS 事件，将来再 从 CooldownStart / CooldownEnd 引脚往外执行。

需要：
在蓝图里显示成 带多个输出引脚的异步节点
对象 活得比一次函数调用更长（一直监听 ASC）
用 BlueprintAssignable 委托 当节点的输出执行引脚
用完能 EndTask + SetReadyToDestroy() 正确收尾
UBlueprintAsyncActionBase 就是 UE 为这类模式提供的基类。

*/

/*
Q1:「异步」从翻译上是什么意思？

A1:「异步」不是「多线程」，而是：现在下单监听，结果稍后再通过委托告诉你（不同步、不同时刻完成）。
*/


/*
Q2:为什么叫「static 工厂」？

A1:static：不依赖已有实例，用 类名::WaitForCooldownChange(...) 就能调（蓝图异步节点的标准入口）。
A2:工厂：这个函数 负责创建并配置好对象，再交出去，而不是让外面自己 new
*/

/*
Q3：两条路径谁调 EndTask？

A3：
情况					谁调 EndTask
ASC / Tag 无效 			C++ 工厂里立刻调
创建成功				蓝图里你必须自己调

成功时对象会一直挂着监听，直到有人调 EndTask()。
*/