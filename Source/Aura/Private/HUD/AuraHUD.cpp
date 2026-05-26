


#include "HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams){
    if(OverlayWidgetController == nullptr){
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerParams(WCParams);
        OverlayWidgetController->BindCallbacksToDependencies();
    }
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS){
    
    /**查看蓝图中是否设置OverlayWidgetControllerClass和OverlayWidgetClass*/
    checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass not found"));
    checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass not found"));
    
    /**创建OverlayWidget，并设置WidgetController，在这里WidgetController是OverlayWidgetController*/
    UUserWidget* Widget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<UAuraUserWidget>(Widget);
    const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
    UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WCParams);

    OverlayWidget->SetWidgetController(WidgetController);
    WidgetController->BroadcastInitialValues();
   
    OverlayWidget->AddToViewport();

}
