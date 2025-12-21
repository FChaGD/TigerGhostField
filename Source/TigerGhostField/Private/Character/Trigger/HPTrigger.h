#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HPTrigger.generated.h"
#include "ITriggerObserver.h"

UCLASS(BlueprintType)
class TIGERGHOSTFIELD_API UHPTrigger : public UObject, public ITriggerObserver
{
    GENERATED_BODY()

public:
    virtual void OnStatChanged(FName StatName, class ABaseCharacter* Owner) override;

};
