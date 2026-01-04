// UDA_RaceForm.cpp
// Author: kwak hyonbin
// Description: Race form validation implementation.

#include "UDA_RaceForm.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstance.h"

bool UDA_RaceForm::IsValidForm() const
{
    // NOTE: For current milestone, only SkeletalMesh is required for stable transform.
    //       Other fields (AnimClass / capsule / movement / content refs) will be validated later.
    return SkeletalMesh != nullptr;
}
