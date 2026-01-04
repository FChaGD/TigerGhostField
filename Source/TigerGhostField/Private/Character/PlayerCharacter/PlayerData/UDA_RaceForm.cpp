// Author: kwak hyonbin
// Description: Race form validation implementation.

#include "UDA_RaceForm.h"
#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMesh.h"

bool UDA_RaceForm::IsValidForm() const
{
    // NOTE: Minimal validation only. Extend if your design requires stricter checks.
    if (!SkeletalMesh)
    {
        return false;
    }

    if (!AnimClass)
    {
        return false;
    }

    return true;
}
