// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Character/CyberbowlCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/CBCharacterMovementComponent.h"
#include "PlayerController/ThirdPersonPlayerController.h"

//////////////////////////////////////////////////////////////////////////
// ACyberbowlCharacter

ACyberbowlCharacter::ACyberbowlCharacter()
{

}

ACyberbowlCharacter::ACyberbowlCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCBCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACyberbowlCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACyberbowlCharacter::Dash);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACyberbowlCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACyberbowlCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACyberbowlCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACyberbowlCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACyberbowlCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACyberbowlCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACyberbowlCharacter::OnResetVR);
}

void ACyberbowlCharacter::Jump()
{
	auto CBCharMoveCmp = Cast<UCBCharacterMovementComponent>(GetCharacterMovement());
	if(!CBCharMoveCmp)
	{
		UE_LOG(LogActor, Error, TEXT("CyberbowlCharacter: CBCharacterMovementCmp not found"));
		return;
	}

	if (CBCharMoveCmp->GetCBMovementMode() == ECBMovementMode::CBMOVE_DoubleJump
		|| CBCharMoveCmp->GetCBMovementMode() == ECBMovementMode::CBMOVE_Wallrun)
	{
		return;
	}
	
	Super::Jump();

	if(CBCharMoveCmp->GetCBMovementMode() == ECBMovementMode::CBMOVE_Jump)
	{
		CBCharMoveCmp->SetCBMovementMode(ECBMovementMode::CBMOVE_DoubleJump);
		return;
	}
	
	CBCharMoveCmp->SetCBMovementMode(ECBMovementMode::CBMOVE_Jump);
}

void ACyberbowlCharacter::Freeze_Implementation(AActor* instigtr)
{
	if(auto instigatorAsChar = Cast<ACyberbowlCharacter>(instigtr))
	{
		if (auto instigatorController = Cast<AThirdPersonPlayerController>(instigatorAsChar->Controller))
		{
			auto targetController = Cast<AThirdPersonPlayerController>(this->Controller);
			if(instigatorController->currPlayerTeam == targetController->currPlayerTeam)
			{
				return;
			}
		}
	}
	
	GetCharacterMovement()->StopMovementImmediately();
	DisableInput(Cast<APlayerController>(Controller));
	
}

void ACyberbowlCharacter::UnFreeze_Implementation()
{
	EnableInput(Cast<APlayerController>(Controller));

}

void ACyberbowlCharacter::Dash()
{
	auto CBCharMoveCmp = Cast<UCBCharacterMovementComponent>(GetCharacterMovement());
	if (!CBCharMoveCmp)
	{
		UE_LOG(LogActor, Error, TEXT("CyberbowlCharacter: CBCharacterMovementCmp not found"));
		return;
	}

	auto cooldownComponent = FindComponentByClass<UCooldownComponent>();
	if (!cooldownComponent)
	{
		UE_LOG(LogActor, Error, TEXT("CyberbowlCharacter: CoolDownComponent not found"));
		return;	
	}
	
	if(cooldownComponent->IsDashReady())
	{
		CBCharMoveCmp->SetCBMovementMode(ECBMovementMode::CBMOVE_Dash);	
	}
}

void ACyberbowlCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACyberbowlCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACyberbowlCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}


void ACyberbowlCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACyberbowlCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACyberbowlCharacter::MoveForward(float Value)
{
	//if ((Controller != NULL) && (Value != 0.0f))
	//{
	//	// find out which way is forward
	//	const FRotator Rotation = Controller->GetControlRotation();
	//	const FRotator YawRotation(0, Rotation.Yaw, 0);

	//	// get forward vector
	//	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//	AddMovementInput(Direction, Value);
	//}
}

void ACyberbowlCharacter::MoveRight(float Value)
{
	//if ( (Controller != NULL) && (Value != 0.0f) )
	//{
	//	// find out which way is right
	//	const FRotator Rotation = Controller->GetControlRotation();
	//	const FRotator YawRotation(0, Rotation.Yaw, 0);
	//
	//	// get right vector 
	//	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//	// add movement in that direction
	//	AddMovementInput(Direction, Value);
	//}
}
