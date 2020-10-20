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

    AIStimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Stimulus Source"));
    
	// Set field default values
    MoveSpeed = 100.f;
    LookSpeed = 1.f;
    SprintMultiplier = 2.f;
    SprintSpeed = 0.f;
    WalkSpeed = 0.f;
}

void APlayerCharacterBase::BeginPlay()
{
    Super::BeginPlay();

    // Calculate movement speeds
    WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    SprintSpeed = WalkSpeed * SprintMultiplier;
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

    PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ACharacterBase::BeginReload);

    PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::SprintStart);
    PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacterBase::SprintEnd);

    // Casts crouch functions to empty parameter functions of ACharacter
    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, (void(ACharacter::*)())&ACharacter::Crouch);
    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, (void(ACharacter::*)())&ACharacter::UnCrouch);
}

void APlayerCharacterBase::OnDeath()
{
    GetController<APlayerLevelController>()->HideHUD();
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

void APlayerCharacterBase::SprintStart()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacterBase::SprintEnd()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}