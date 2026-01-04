// Author: kwak hyonbin
// Description: Shared race type definitions for player race/form systems.

#pragma once

#include "CoreMinimal.h"
#include "RaceTypes.generated.h"

UENUM(BlueprintType)
enum class ERaceType : uint8
{
    RaceA UMETA(DisplayName = "RaceA"),
    RaceB UMETA(DisplayName = "RaceB"),
    RaceC UMETA(DisplayName = "RaceC"),
    MAX
};
