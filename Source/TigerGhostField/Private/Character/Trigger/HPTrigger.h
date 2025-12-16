#pragma once

#include "CoreMinimal.h"
#include "HPTrigger.generated.h"

UCLASS(BlueprintType)
class TIGERGHOSTFIELD_API UHPTrigger : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
    int32 Guard;
};
