// Author: kwak hyonbin
// Description: Race form data asset defining appearance, movement, collision, and per-race content references (e.g., attack data assets).

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RaceTypes.h"
#include "UDA_RaceForm.generated.h"

class USkeletalMesh;
class UAnimInstance;
class UDA_AttackSet;      // Example: per-race attack set data asset
class UDA_SkillSet;       // Example: per-race skill set data asset
class UDA_AttributeSet;   // Example: per-race attributes data asset

UCLASS(BlueprintType)
class TIGERGHOSTFIELD_API UDA_RaceForm : public UDataAsset
{
    GENERATED_BODY()

public:
    // NOTE: This key should match the keys used by the owning race component
    //       (URaceFormComponent::OwnedRaceForms) on the character.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race")
    ERaceType RaceType = ERaceType::Human;

#pragma region Form_Appearance
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Appearance")
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Appearance")
    TSubclassOf<UAnimInstance> AnimClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Appearance")
    FVector MeshRelativeLocation = FVector(0.0f, 0.0f, -88.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Appearance")
    FRotator MeshRelativeRotation = FRotator::ZeroRotator;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Appearance")
    FVector MeshRelativeScale = FVector(1.0f, 1.0f, 1.0f);
#pragma endregion

#pragma region Form_Collision
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Collision", meta = (ClampMin = "1.0"))
    float CapsuleRadius = 34.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Collision", meta = (ClampMin = "1.0"))
    float CapsuleHalfHeight = 88.0f;
#pragma endregion

#pragma region Form_Movement
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Movement", meta = (ClampMin = "0.0"))
    float MaxWalkSpeed = 600.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Movement", meta = (ClampMin = "0.0"))
    float JumpZVelocity = 420.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Form|Movement", meta = (ClampMin = "0.0"))
    float RotationRateYaw = 540.0f;
#pragma endregion

#pragma region Content_References
    // NOTE: Use SoftObjectPtr if you want to store "path reference" without forcing load at startup.

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Content|Combat")
    //TSoftObjectPtr<UDA_AttackSet> AttackSet;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Content|Skill")
    //TSoftObjectPtr<UDA_SkillSet> SkillSet;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Content|Attributes")
    //TSoftObjectPtr<UDA_AttributeSet> AttributeSet;
#pragma endregion

public:
    /**
     * Summary: Returns whether this form contains the minimum required data to be applied.
     * @return True if required assets are present.
     */
    UFUNCTION(BlueprintCallable, Category = "Race")
    bool IsValidForm() const;
};
