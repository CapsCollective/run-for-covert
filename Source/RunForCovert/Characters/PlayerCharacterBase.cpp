// Caps Collective 2020


#include "PlayerCharacterBase.h"
#include "../Controllers/PlayerLevelController.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // Setup components
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(GetCapsuleComponent());

    Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
    Arms->SetupAttachment(Camera);

    MeshOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MeshOffset"));
    MeshOffset->SetupAttachment(GetCapsuleComponent());

    AIStimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Stimulus Source"));
    
	// Set field default values
    MoveSpeed = 100.f;
    LookSpeed = 1.f;
    SprintMultiplier = 2.f;
    StandingHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    CrouchHeight = 50.f;
}

void APlayerCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    // Apply the sprint multiplier to the character speed
    ApplySprintMultiplier(SprintMultiplier);
}

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacterBase::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacterBase::MoveRight);

    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacterBase::LookUp);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacterBase::LookRight);

    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::Jump);

    PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::FireStart);
    PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &APlayerCharacterBase::FireEnd);

    PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, (void(ACharacter::*)())&ACharacterBase::BeginReload);

    PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ACharacterBase::SprintStart);
    PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ACharacterBase::SprintEnd);

    // Casts crouch functions to empty parameter functions of ACharacter
    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::CrouchStart);
    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &APlayerCharacterBase::CrouchEnd);
}

void APlayerCharacterBase::OnDeath()
{
    // Hide the HUD if the player is posessed by the local player
    if (IsLocallyControlled())
    {
        GetController<APlayerLevelController>()->HideHUD();
    }
    Super::OnDeath();
}

FVector APlayerCharacterBase::GetAimVector()
{
    return Camera->GetForwardVector();
}

void APlayerCharacterBase::ApplyRecoil(FRotator& Recoil)
{
    // Apply the recoil to the character's local rotation
    AddControllerPitchInput(Recoil.Pitch);
    AddControllerYawInput(Recoil.Yaw);
}

void APlayerCharacterBase::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount * MoveSpeed);
}

void APlayerCharacterBase::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount * MoveSpeed);
}

void APlayerCharacterBase::LookUp(float Amount)
{
    AddControllerPitchInput(Amount * LookSpeed);
}

void APlayerCharacterBase::LookRight(float Amount)
{
    AddControllerYawInput(Amount * LookSpeed);
}

void APlayerCharacterBase::FireStart()
{
    if (!GetGun()) { return; }

    // Only cancel the reload if the gun is not empty
    if (GetGun()->HasAmmo())
    {
        CancelReload();
    }
    GetGun()->SetTriggerDown(true);
}

void APlayerCharacterBase::FireEnd()
{
    if (!GetGun()) { return; }
    GetGun()->SetTriggerDown(false);
}

void APlayerCharacterBase::CrouchStart()
{
    PerformCrouch(true, CrouchHeight, StandingHeight - CrouchHeight);
}

void APlayerCharacterBase::CrouchEnd()
{
    PerformCrouch(false, StandingHeight, CrouchHeight - StandingHeight);
}

void APlayerCharacterBase::PerformCrouch(bool bCrouch, float HalfHeight, float Offset)
{
    // Run crouch on the current client and forward it for the relevant local role
    ApplyCrouch(bCrouch, HalfHeight, Offset);
    if (HasAuthority())
    {
        MulticastApplyCrouch(bCrouch, HalfHeight, Offset);
    }
    else
    {
        ServerApplyCrouch(bCrouch, HalfHeight, Offset);
    }
}

void APlayerCharacterBase::ApplyCrouch(bool bCrouch, float HalfHeight, float Offset)
{
    SetCrouching(bCrouch);
    GetCapsuleComponent()->SetCapsuleHalfHeight(HalfHeight);
    MeshOffset->SetRelativeLocation(MeshOffset->GetRelativeLocation() + FVector(0.f, 0.f, Offset));
    UpdateComponentTransforms();
}

void APlayerCharacterBase::ServerApplyCrouch_Implementation(bool bCrouch, float HalfHeight, float Offset)
{
    ApplyCrouch(bCrouch, HalfHeight, Offset);
}

void APlayerCharacterBase::MulticastApplyCrouch_Implementation(bool bCrouch, float HalfHeight, float Offset)
{
    // Run crouch on clients only
    if (!HasAuthority()) { ApplyCrouch(bCrouch, HalfHeight, Offset); }
}