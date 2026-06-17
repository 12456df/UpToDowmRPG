



#include "Actor/AuraProjectile.h"



#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Aura/Aura.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraProjectile::AAuraProjectile()

{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()

{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}



void AAuraProjectile::Destroyed()

{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}
	}
	Super::Destroyed();
}


void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)

{
	AActor* SourceAvatar = GetOwner();                       // 敌人：owner 就是角色本身
	if (const APlayerState* PS = Cast<APlayerState>(GetOwner()))
	{
    	SourceAvatar = PS->GetPawn();                         // 玩家：owner 是 PlayerState，取其控制的角色
	}
	if (OtherActor == SourceAvatar) return;  

	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}
	if (HasAuthority())
	{

		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
	else
	{
		bHit = true;
	}
}




/*
Q1：LoopingSoundComponent->Stop() 空指针崩溃
A1：弹丸生成位置错误（问题 1 导致），一出生就和发射者碰撞，
OnSphereOverlap 在 BeginPlay 的 SpawnSoundAttached 还没完成时就被触发，
组件还是 null 就调了 Stop()。
S1：调用前判空（兜底保护）。


Q2：火球撞到自己消失，飞不出去
A2：用 GetOwner() 反查真正的发射者角色，与生成位置无关：
S2：AActor* SourceAvatar = GetOwner();                       // 敌人：owner 就是角色
if (const APlayerState* PS = Cast<APlayerState>(GetOwner()))
{
    SourceAvatar = PS->GetPawn();                         // 玩家：owner 是 PlayerState，取其 Pawn
}
if (OtherActor == SourceAvatar) return;
*/











