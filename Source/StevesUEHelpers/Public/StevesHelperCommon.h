#pragma once
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
    Mouse,
    Keyboard,
    Gamepad,
    Unknown
};

UENUM(BlueprintType)
enum class EInputModeChange : uint8
{
    DoNotChange UMETA(DisplayName="No Change"),
    UIOnly UMETA(DisplayName="UI Only"),
    GameAndUI UMETA(DisplayName="Game And UI"),
    GameOnly UMETA(DisplayName="Game Only")
};
    
UENUM(BlueprintType)
enum class EMousePointerVisibilityChange : uint8
{
    DoNotChange UMETA(DisplayName="No Change"),
    Visible UMETA(DisplayName="Pointer Visible"),
    Hidden UMETA(DisplayName="Pointer Hidden")
};

UENUM(BlueprintType)
enum class EGamePauseChange : uint8
{
    DoNotChange UMETA(DisplayName="No Change"),
    Paused UMETA(DisplayName="Pause Game"),
    Unpaused UMETA(DisplayName="Unpause Game")
};

