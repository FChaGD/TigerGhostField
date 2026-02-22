// UDA_RaceForm.cpp
// Author: kwak hyonbin
// Description: Race form validation implementation.

#include "UDA_RaceForm.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"

bool UDA_RaceForm::IsValidForm() const
{
    // English comment: Minimal requirement is SkeletalMesh.
    return (SkeletalMesh != nullptr);
}