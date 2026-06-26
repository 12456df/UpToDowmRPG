


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"
#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Game/AuraGameModeBase.h"
#include "Player/AuraPlayerState.h"
#include "HUD/AuraHUD.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject){
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}


void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	
	AActor* AvatarActor = ASC->GetAvatarActor();
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(!CharacterClassInfo) return;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);//使用CharacterClassInfo的内置函数GetClassDefaultInfo(ECharacterClass CharacterClass)获取CharacterClassDefaultInfo
	
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}
void UAuraAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(const AActor* Actor1, const AActor* Actor2)
{
	bool bBothEnemy = Actor1->ActorHasTag(FName("Enemy")) && Actor2->ActorHasTag(FName("Enemy"));
	bool bBothPlayer = Actor1->ActorHasTag(FName("Player")) && Actor2->ActorHasTag(FName("Player"));
	bool bFriend = bBothEnemy || bBothPlayer;
	return !bFriend;
}
/*
Q1:WorldContextObject 是什么
A1:WorldContextObject 不是"因为 UObject 是 Character 的父类"所以能传。它的含义是："帮我找到你所在的 World（世界）"。

UE 里很多全局函数（GetGameMode、GetPlayerController、SpawnActor 等）需要知道"在哪个世界里操作"。但静态函数没有 this，不属于任何 Actor，所以需要借一个身处那个世界的对象来定位 World。

引擎内部大致是这样拿 World 的：
UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
所以它不在乎你传的是 Character、Actor 还是 Widget——只要这个对象能找到一个 UWorld，就行。你传 this（Enemy），Enemy 在世界里，自然能找到 World。传 PlayerController、GameState 也都可以。

在蓝图里，继承 UBlueprintFunctionLibrary 的函数如果第一个参数叫 WorldContextObject，引擎会自动帮你填上，蓝图里看不到这个引脚——这就是为什么蓝图调用时不需要手动连。
--------------------------------
Q2:Enemy 为什么能拿到 AuraGameModeBase
A2:AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
GetGameMode 不是 Enemy 的函数，是引擎的全局函数。它的逻辑是：
WorldContextObject（Enemy）→ 找到 World → World 里有唯一的 GameMode → 返回它
GameMode 是整个关卡的"规则管理者"，任何在这个 World 里的对象都能通过 GetGameMode 拿到它。跟谁调用无关——Enemy 能拿到，PlayerController 也能拿到，一个 Widget 也能拿到。

注意：GameMode 只存在于 Server 端。如果这个函数在 Client 上调用，GetGameMode 返回 null。这也是为什么代码里有 if (AuraGameMode == nullptr) return; 的保护。敌人初始化属性只在 Server 跑，所以没问题。
--------------------------------
Q3:GE 标准应用流程（MakeEffectContext → MakeOutgoingSpec → Apply）
A3:
第一步：MakeEffectContext —— "谁造成的"
FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
Context.AddSourceObject(AvatarActor);

创建一个上下文对象，记录"这次效果的来源是谁"。AddSourceObject 把 AvatarActor（敌人自己）标记为来源。
为什么需要：GE 执行时（比如在 PostGameplayEffectExecute 里），你能通过 Context 反查"是谁施加的这个效果"。

第二步：MakeOutgoingSpec —— "做什么 + 等级多少"
const FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GEClass, Level, Context);

把三样东西打包成一份规格单（Spec）：
GEClass：用哪个 GE（图纸）
Level：等级（影响 Scalable Float 的曲线取值）
Context：上一步做的上下文
为什么不直接 Apply 一个 GE 类：因为同一个 GE 类可能被不同等级、不同来源使用。Spec 是"一次具体的使用"，GE 类是"通用模板"。

第三步：ApplyGameplayEffectSpecToSelf —— "作用到谁身上"
ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
把规格单真正执行到 ASC 身上，修改属性。
ToSelf 表示"施加到自己"——因为这里是敌人初始化自己的属性，来源和目标是同一个 ASC。如果是火球打别人，用的是 TargetASC->ApplyGameplayEffectSpecToSelf（在目标的 ASC 上调用）。
*/