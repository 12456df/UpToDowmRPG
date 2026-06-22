


#include "Player/AuraPlayerController.h"
#include "Input/AuraInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "UI/Widget/DamageTextComponent.h"
#include "GameFramework/Character.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
    CursorTrace();
    AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bIsBlockedHit, bool bIsCriticalHit)
{
    if(IsValid(TargetCharacter) && IsValid(DamageTextComponentClass) && IsLocalController())
    {
        UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
        DamageTextComponent->RegisterComponent();
        DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamageTextComponent->SetDamageText(DamageAmount, bIsBlockedHit, bIsCriticalHit);
    }
}

void AAuraPlayerController::CursorTrace(){
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if(!CursorHit.bBlockingHit) return;
    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();
    

    if(LastActor != ThisActor)
    {
        if(LastActor) LastActor->UnHighlightActor();
        if(ThisActor) ThisActor->HighlightActor();
        
    }
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
    check(AuraContext);
    //拿到「当前这名本地玩家」专属的「增强输入子系统」
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
   if(Subsystem){//将「增强输入上下文」添加到「增强输入子系统」中

    Subsystem->AddMappingContext(AuraContext, 0);
    
   }

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;
    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    //设置输入模式
    SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
    UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
    AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
    AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
    AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
    AuraInputComponent->BindAbilityActions(AuraInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}
void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue){
    const FVector2D MoveDirection = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    if(APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, MoveDirection.Y);
        ControlledPawn->AddMovementInput(RightDirection, MoveDirection.X);
    }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    //GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Green, FString::Printf(TEXT("AbilityInputTagPressed: %s"), *InputTag.ToString()));
    if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        bTargeting = ThisActor?true:false;
        bAutoRunning = false;
    }
}
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))   
    {
        if(GetASC())GetASC()->AbilityInputTagReleased(InputTag);
        return;
    }
    if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	if (!bTargeting && !bShiftKeyDown)
    {
        const APawn* ControlledPawn = GetPawn<APawn>();
        if(FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                if(NavPath->IsValid() && NavPath->PathPoints.Num() > 0)
                {
                    Spline->ClearSplinePoints();
                    for(const FVector& PointLocation : NavPath->PathPoints)
                    {
                        Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
                    }
                    if (NavPath->IsValid() && NavPath->PathPoints.Num() > 0)
                    {
                        CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
                        bAutoRunning = true;
                    }
                }
            }
        }

        FollowTime = 0.f;
        bTargeting = false;
    }
}
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        if(GetASC())GetASC()->AbilityInputTagHeld(InputTag);
        return;
    }
    
    if(bTargeting || bShiftKeyDown)
    {
        if(GetASC())GetASC()->AbilityInputTagHeld(InputTag);
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();

        if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
        if(APawn* ControlledPawn = GetPawn<APawn>())
        {
            const FVector Direction = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(Direction);
        }
    }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
    if(AuraAbilitySystemComponent == nullptr)
    {
        AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }
    return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
    if(!bAutoRunning) return;
    if(APawn* ControlledPawn = GetPawn<APawn>())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
        const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
        ControlledPawn->AddMovementInput(Direction);
        
        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
        if(DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}