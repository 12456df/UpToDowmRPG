


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
    CursorTrace();
}
void AAuraPlayerController::CursorTrace(){
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if(!CursorHit.bBlockingHit) return;
    LastActor = CurrentActor;
    CurrentActor = CursorHit.GetActor();
    /**
    * A.LastActor is nullptr,CurrentActor is nullptr
      -> do nothing
    * B.LastActor is nullptr,CurrentActor is valid
      -> Highlight CurrentActor
    * C.LastActor is valid,CurrentActor is nullptr
      -> Unhighlight LastActor
    * D.LastActor is valid,CurrentActor is valid
      -> if(LastActor != CurrentActor)
         -> Highlight CurrentActor
         -> Unhighlight LastActor
    */    
    if(LastActor == nullptr && CurrentActor == nullptr) return;
    else if(LastActor == nullptr && CurrentActor != nullptr) CurrentActor->HighlightActor();
    else if(LastActor != nullptr && CurrentActor == nullptr) LastActor->UnhighlightActor();
    else if(LastActor != nullptr && CurrentActor != nullptr && LastActor != CurrentActor)
    {
        LastActor->UnhighlightActor();
        CurrentActor->HighlightActor();
    }
    else{
        return;
    }
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
    check(AuraContext);
    //拿到「当前这名本地玩家」专属的「增强输入子系统」
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(Subsystem);
    //将「增强输入上下文」添加到「增强输入子系统」中
    Subsystem->AddMappingContext(AuraContext, 0);

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
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
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