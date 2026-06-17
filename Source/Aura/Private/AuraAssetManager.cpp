


#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"
UAuraAssetManager& UAuraAssetManager::Get()
{
    check(GEngine);
    UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();
    FAuraGameplayTags::InitializeNativeGameplayTags();
    /*FGameplayAbilityTargetData_SingleTargetHit 这类结构，网络上传时要序列化，让 GAS 认识这些类型，知道怎么打包、怎么解包*/
    UAbilitySystemGlobals::Get().InitGlobalData();
}