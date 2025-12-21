#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITriggerObserver.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UTriggerObserver : public UInterface
{
    GENERATED_BODY()
};

class TIGERGHOSTFIELD_API ITriggerObserver
{
    GENERATED_BODY()

public:
    virtual void OnStatChanged(FName StatName, class ABaseCharacter* Owner) = 0;
};
