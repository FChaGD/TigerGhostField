// Author: kwa// Author: kwak hyonbin
// Description: Shared race type definitions for player race/form systems.

#pragma once

#include "CoreMinimal.h"
#include "RaceTypes.generated.h"

// Race type definition for player race/form systems.
UENUM(BlueprintType)
enum class ERaceType : uint8
{
    Human     UMETA(DisplayName = "Human"),     // Human race
    Wraith    UMETA(DisplayName = "Wraith"),    // Wraith race
    Yokai     UMETA(DisplayName = "Yokai"),     // Yokai race
    MAX       UMETA(Hidden)                     // Sentinel value
};
