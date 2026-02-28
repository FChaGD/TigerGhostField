// RaceFormComponent.h
// Author: refactor assistant
// Description: Handles all "race" related transformation logic for a
//              character, including mesh/animation swaps, capsule/movement
//              adjustments, and storing available race forms.  This is
//              pulled out of ABaseCharacter in order to improve separation
//              of concerns.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../PlayerCharacter/PlayerData/Race/RaceTypes.h"
#include "RaceFormComponent.generated.h"

class UDA_RaceForm;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIGERGHOSTFIELD_API URaceFormComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URaceFormComponent();

    /**
     * Attempts to transform the owning character to the specified race
     * type. Returns true if the change succeeded or the target was already
     * applied.
     */
    bool TransformToRace(ERaceType TargetRaceType);

    /**
     * Applies the details contained in a race form asset to the owning
     * character.  Caller is responsible for verifying the asset is valid.
     */
    void ApplyRaceForm(const UDA_RaceForm* Form);

    ERaceType GetCurrentRaceType() const { return CurrentRaceType; }

    /**
     * Accessor for the configured default race.  Useful for external logic
     * that needs to know the fallback without performing a transformation.
     */
    ERaceType GetDefaultRaceType() const { return DefaultRaceType; }

protected:
    virtual void BeginPlay() override;

private:
    ERaceType CurrentRaceType = ERaceType::Human;

    // ---------- editable via editor ------------------------------------
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race",
              meta = (AllowPrivateAccess = "true"))
    ERaceType DefaultRaceType = ERaceType::Human;

    UPROPERTY(EditDefaultsOnly, Category = "Race",
              meta = (AllowPrivateAccess = "true"))
    TMap<ERaceType, TObjectPtr<UDA_RaceForm>> OwnedRaceForms;

    UPROPERTY(VisibleInstanceOnly, Category = "Race")
    bool bHasAppliedRaceForm = false;
};
