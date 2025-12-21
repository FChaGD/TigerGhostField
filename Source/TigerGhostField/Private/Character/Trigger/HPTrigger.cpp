#include "HPTrigger.h"
#include "Character/ABaseCharacter.h"

class ABaseCharacter;

void UHPTrigger::OnStatChanged(FName StatName, ABaseCharacter* Owner)
{
    UE_LOG(LogTemp, Warning, TEXT("HPTrigger: %s changed on %s"), *StatName.ToString(), *Owner->GetName());
}