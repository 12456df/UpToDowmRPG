

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
class UAuraWidgetController;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class APlayerController;
class APlayerState;
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	
protected:
	
private:
     /**创建OverlayWidget*/
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

    /**创建OverlayWidgetController*/
    UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
    UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
