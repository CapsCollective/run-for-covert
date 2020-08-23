// Caps Collective 2020


#include "PlayerCharacterBase.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"

#define OUT

APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

    // Setup components
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(GetCapsuleComponent());

    Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
    Arms->SetupAttachment(Camera);

	// Set field default values
    MoveSpeed = 100.f;
    LookSpeed = 1.f;
    SprintMultiplier = 2.f;
    DefaultCapsuleHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacterBase::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacterBase::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacterBase::LookUp);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacterBase::LookRight);

    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::Jump);

    PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::Fire);

    PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::SprintStart);
    PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacterBase::SprintEnd);

    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APlayerCharacterBase::CrouchStart);
    PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &APlayerCharacterBase::CrouchEnd);
}

void APlayerCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void APlayerCharacterBase::Fire()
{
    if (!GetGun()) { return; }
    GetGun()->Fire(GetController(), Camera->GetForwardVector());
}

void APlayerCharacterBase::SprintStart()
{
    GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
}

void APlayerCharacterBase::SprintEnd()
{
    GetCharacterMovement()->MaxWalkSpeed /= SprintMultiplier;
}

void APlayerCharacterBase::CrouchStart()
{
    GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight/2);
}

void APlayerCharacterBase::CrouchEnd()
{
    GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHeight);
}