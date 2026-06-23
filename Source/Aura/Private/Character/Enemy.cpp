


#include "Character/Enemy.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Aura/Aura.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
AEnemy::AEnemy(){
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
}

void AEnemy::PossessedBy(AController* NewController){
	Super::PossessedBy(NewController);
	if(!HasAuthority())
	{
		return;
	}
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AEnemy::BeginPlay(){
	Super::BeginPlay();
	InitAbilityActorInfo();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	if(HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		/*
		委托1：FOnGameplayAttributeValueChange
			声明类型：引擎 AbilitySystemComponent.h 里用宏声明（FOnGameplayAttributeValueChange）
			创建实例：ASC 内部为每个 Attribute 维护一个，GetGameplayAttributeValueChangeDelegate(Attribute) 把它取出来
			绑定监听：.AddLambda([this](const FOnAttributeChangeData& Data){...})
			广播触发：当 Health 属性被 GE 改变时，ASC 自动 Broadcast，从而调用你的 lambda
		委托2：FOnAttributeChangedDelegateSignature
			声明类型：OverlayWidgetController.h 第33行 DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedDelegateSignature, float, NewValue)
			创建实例：Enemy.h：FOnAttributeChangedDelegateSignature OnHealthChanged;
			绑定监听：UI 血条蓝图（WBP_EnemyHealthBar）里绑定 OnHealthChanged
			广播触发：OnHealthChanged.Broadcast(Data.NewValue)
		*/
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this, AuraAS](const FOnAttributeChangeData& /*Data*/)
			{
				OnHealthChanged.Broadcast(AuraAS->GetHealth());
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this, AuraAS](const FOnAttributeChangeData& /*Data*/)
			{
				OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
			}
		);
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, 
			&AEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AEnemy::InitAbilityActorInfo(){
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if(HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AEnemy::HighlightActor(){
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
    bHighlighted = true;
}

void AEnemy::UnHighlightActor(){
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
	Weapon->SetRenderCustomDepth(false);
	Weapon->SetCustomDepthStencilValue(0);
    bHighlighted = false;
}

int32 AEnemy::GetPlayerLevel()
{
	return Level;
}
void AEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AEnemy::SetCombatTarget_Implementation(AActor* inCombatTarget)
{
	CombatTarget = inCombatTarget;
}
AActor* AEnemy::GetCombatTarget_Implementation(){
	return CombatTarget;
}
void AEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if(NewCount > 0)
	{
		bHitReacting = true;
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else
	{
		bHitReacting = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
	
}

/*
Q1：给 BP_EnemyBase 加了 WidgetComponent（HealthBar）后，PIE 里 client 和 server 头顶都看不到血条。
A1：HealthBar 组件的 Space 被设成了 World（世界空间）。World 空间的 widget 是 3D 面片，
会因为背对相机（背面剔除）、距离缩小、被网格遮挡、受光照影响等原因变得看不见。
S1：把 HealthBar 组件的 Space 改成 Screen（屏幕空间）。
Screen 空间永远正对相机、永远在最上层、大小固定——这正是头顶血条的标准设置。

Q2：火球命中敌人，PostGameplayEffectExecute 确实触发，
日志显示 magnitude = -10，但 GetHealth() 始终是 106.5，血量和血条都不变。
A2：DefaultVitalAttributes（设置 Health 的 GE）的 Duration Policy 被设成了 Infinite，而不是 Instant。
S2：把 Duration Policy 改成 Instant。
*/