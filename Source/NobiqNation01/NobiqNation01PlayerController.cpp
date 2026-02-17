#include "NobiqNation01PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputCoreTypes.h"
#include <TerrainPatch.h>
#include "NobiqNation01PlayerController.h"

ANobiqNation01PlayerController::ANobiqNation01PlayerController()
{
    // ‚¢‚Ü‚Í‹ó‚ÅOKi‚Ü‚¸ƒŠƒ“ƒN‚ð’Ê‚·j
}

void ANobiqNation01PlayerController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("USING PC: %s"), *GetClass()->GetName());
    UE_LOG(LogTemp, Warning, TEXT("IMC_Default=%s"), *GetNameSafe(IMC_Default));
    UE_LOG(LogTemp, Warning, TEXT("IMC_MouseLook=%s"), *GetNameSafe(IMC_MouseLook));

    if (ULocalPlayer* LP = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (IMC_Default)   Subsystem->AddMappingContext(IMC_Default, 0);
            if (IMC_MouseLook) Subsystem->AddMappingContext(IMC_MouseLook, 1);
        }
    }
}

void ANobiqNation01PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed,
        this, &ANobiqNation01PlayerController::OnLeftClick);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ANobiqNation01PlayerController::OnLeftClick()
{
    UE_LOG(LogTemp, Warning, TEXT("CLICKED"));

    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (!Hit.bBlockingHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("NO HIT"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("HIT actor=%s"), *GetNameSafe(Hit.GetActor()));

    if (ATerrainPatch* Terrain = Cast<ATerrainPatch>(Hit.GetActor()))
    {
        Terrain->RaiseAtWorldPoint(Hit.ImpactPoint, 200.f, 50.f);
    }
}
