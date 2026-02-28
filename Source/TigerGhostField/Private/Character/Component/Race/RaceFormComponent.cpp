// RaceFormComponent.cpp
// Implementation of the race transformation logic.

#include "Character/Component/Race/RaceFormComponent.h"
#include "Character/ABaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DebugHelper.h"

URaceFormComponent::URaceFormComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URaceFormComponent::BeginPlay()
{
    Super::BeginPlay();
    // Ensure race defaults are applied when the game starts.
    // We might want to delay this until the owner is fully initialized.
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        if (!OwnerChar->GetWorld() || !OwnerChar->GetWorld()->IsGameWorld())
        {
            UE_LOG(LogTemp, Warning, TEXT("[Race] BeginPlay cancelled - not in game world"));
            return;
        }
        
        UE_LOG(LogTemp, Log, TEXT("[Race] BeginPlay - scheduling default race transform (DefaultRace: %d)"), 
            static_cast<int32>(DefaultRaceType));
        
        OwnerChar->GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
            UE_LOG(LogTemp, Log, TEXT("[Race] Applying default race form"));
            TransformToRace(DefaultRaceType);
        });
    }
}

bool URaceFormComponent::TransformToRace(ERaceType TargetRaceType)
{
    UE_LOG(LogTemp, Log, TEXT("[Race] Transform initiated (Actor: %s, CurrentRace: %d, TargetRace: %d)"), 
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"), 
        static_cast<int32>(CurrentRaceType), static_cast<int32>(TargetRaceType));

    if (TargetRaceType == CurrentRaceType && bHasAppliedRaceForm)
    {
        UE_LOG(LogTemp, Verbose, TEXT("[Race] Transform skipped - already in target race"));
        return true;
    }

    const TObjectPtr<UDA_RaceForm>* FoundPtr = OwnedRaceForms.Find(TargetRaceType);
    if (!FoundPtr || !(*FoundPtr))
    {
        UE_LOG(LogTemp, Error, TEXT("[Race] Transform failed - no data for race %d"),
            static_cast<int32>(TargetRaceType));
        return false;
    }

    const UDA_RaceForm* Form = (*FoundPtr);
    if (!Form->IsValidForm())
    {
        UE_LOG(LogTemp, Error, TEXT("[Race] Transform failed - invalid form data for race %d"),
            static_cast<int32>(TargetRaceType));
        return false;
    }

    ApplyRaceForm(Form);
    CurrentRaceType = TargetRaceType;
    bHasAppliedRaceForm = true;

    UE_LOG(LogTemp, Log, TEXT("[Race] Transform succeeded (Actor: %s, NewRace: %d)"), 
        GetOwner() ? *GetOwner()->GetName() : TEXT("None"), static_cast<int32>(CurrentRaceType));

    return true;
}

void URaceFormComponent::ApplyRaceForm(const UDA_RaceForm* Form)
{
    if (!Form)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceForm called with null Form"));
        return;
    }

    ABaseCharacter* OwnerChar = Cast<ABaseCharacter>(GetOwner());
    if (!OwnerChar)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceForm - owner is not ABaseCharacter"));
        return;
    }

    USkeletalMeshComponent* MeshComp = OwnerChar->GetMesh();
    if (!MeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceForm - owner %s has no mesh component"), *OwnerChar->GetName());
        return;
    }

    // Apply skeletal mesh
    if (Form->SkeletalMesh)
    {
        UE_LOG(LogTemp, Verbose, TEXT("[Race] Applying skeletal mesh: %s"), *Form->SkeletalMesh->GetName());
#if ENGINE_MAJOR_VERSION >= 5
        MeshComp->SetSkeletalMesh(Form->SkeletalMesh, true);
#else
        MeshComp->SetSkeletalMesh(Form->SkeletalMesh);
        MeshComp->InitAnim(true);
#endif
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Race] ApplyRaceForm - SkeletalMesh is null"));
    }

    // Apply animation class
    if (Form->AnimClass)
    {
        UE_LOG(LogTemp, Verbose, TEXT("[Race] Applying animation class: %s"), *Form->AnimClass->GetName());
        MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        MeshComp->SetAnimInstanceClass(Form->AnimClass);
        MeshComp->InitAnim(true);
    }

    // Apply capsule and mesh transforms
    if (UCapsuleComponent* Cap = OwnerChar->GetCapsuleComponent())
    {
        UE_LOG(LogTemp, Verbose, TEXT("[Race] Applying capsule size (Radius: %.1f, HalfHeight: %.1f)"), 
            Form->CapsuleRadius, Form->CapsuleHalfHeight);
        Cap->SetCapsuleSize(Form->CapsuleRadius, Form->CapsuleHalfHeight, true);

        FVector Loc = Form->MeshRelativeLocation;
        Loc.Z = -Form->CapsuleHalfHeight;
        MeshComp->SetRelativeLocation(Loc);
    }
    else
    {
        MeshComp->SetRelativeLocation(Form->MeshRelativeLocation);
    }

    MeshComp->SetRelativeRotation(Form->MeshRelativeRotation);
    MeshComp->SetRelativeScale3D(Form->MeshRelativeScale);
    UE_LOG(LogTemp, Verbose, TEXT("[Race] Mesh transforms applied (Loc: %.0f,%.0f,%.0f, Rot: %.0f,%.0f,%.0f, Scale: %.1f,%.1f,%.1f)"), 
        Form->MeshRelativeLocation.X, Form->MeshRelativeLocation.Y, Form->MeshRelativeLocation.Z,
        Form->MeshRelativeRotation.Roll, Form->MeshRelativeRotation.Pitch, Form->MeshRelativeRotation.Yaw,
        Form->MeshRelativeScale.X, Form->MeshRelativeScale.Y, Form->MeshRelativeScale.Z);

    // Apply movement parameters
    if (UCharacterMovementComponent* Move = OwnerChar->GetCharacterMovement())
    {
        UE_LOG(LogTemp, Verbose, TEXT("[Race] Applying movement params (MaxWalkSpeed: %.1f, JumpZ: %.1f, RotationRate: %.1f)"), 
            Form->MaxWalkSpeed, Form->JumpZVelocity, Form->RotationRateYaw);
        Move->MaxWalkSpeed = Form->MaxWalkSpeed;
        Move->JumpZVelocity = Form->JumpZVelocity;
        Move->RotationRate = FRotator(0.f, Form->RotationRateYaw, 0.f);
    }

    UE_LOG(LogTemp, Log, TEXT("[Race] Race form applied successfully (Actor: %s, Mesh: %s, AnimClass: %s)"), 
        *OwnerChar->GetName(), 
        Form->SkeletalMesh ? *Form->SkeletalMesh->GetName() : TEXT("None"),
        Form->AnimClass ? *Form->AnimClass->GetName() : TEXT("None"));
}
