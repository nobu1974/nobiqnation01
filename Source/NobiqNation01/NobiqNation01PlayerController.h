#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "NobiqNation01PlayerController.generated.h"

UCLASS()
class NOBIQNATION01_API ANobiqNation01PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ANobiqNation01PlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> IMC_Default;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> IMC_MouseLook;

    void OnLeftClick();
};
