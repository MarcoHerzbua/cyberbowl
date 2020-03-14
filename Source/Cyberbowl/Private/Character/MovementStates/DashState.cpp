#include "Character/MovementStates/DashState.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"


void UDashState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	UBaseMovementState::InitializeState(moveComponent);
	DefaultGravityScale = MovementComponent->GravityScale;
}

void UDashState::Activate(ECBMovementMode previousMode)
{
	UBaseMovementState::Activate(previousMode);

	float forwardAxisInput = InputComponent->GetAxisValue("MoveForward");
	float rightAxisInput = InputComponent->GetAxisValue("MoveRight");
	FVector axisInput = FVector(forwardAxisInput, rightAxisInput, 0.f);
	FRotator viewRotation = MovementComponent->GetPawnOwner()->GetControlRotation();

	//Rotate the inputAxis to match with the View Rotation of the camera
	axisInput = axisInput.RotateAngleAxis(viewRotation.Yaw, FVector::UpVector);

	float finalDashDuration = MovementComponent->DashDuration;
	if (axisInput.Size() == 0)
	{
		DashDirection = FVector::UpVector;
		//decrease dash duration, otherwise character chrashes into roof
		finalDashDuration /= 2.f;

		//FIX: This is needed, otherwise character will not dash upwards when standing still on ground
		MovementComponent->DoJump(false);
	}
	else
	{
		DashDirection = axisInput;
	}
	
	//InitialVelocity = MovementComponent->Velocity;
	PreviousMovementMode = previousMode;
	MovementComponent->GravityScale = 0.f;
	MovementComponent->StopMovementImmediately();
	MovementComponent->GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UDashState::StopDash, finalDashDuration);
}

void UDashState::Deactivate()
{
	UBaseMovementState::Deactivate();

	MovementComponent->GravityScale = DefaultGravityScale;
	MovementComponent->GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	DashDirection = FVector::ZeroVector;
	PreviousMovementMode = ECBMovementMode::CBMOVE_Running;
}

void UDashState::OnTick(float DeltaTime)
{
	MovementComponent->Velocity = DashDirection * MovementComponent->DashForce;
}

void UDashState::StopDash()
{
	FVector newVelocity = MovementComponent->Velocity.GetSafeNormal() * MovementComponent->MaxWalkSpeed;
	MovementComponent->StopMovementImmediately();
	MovementComponent->Velocity = newVelocity;

	MovementComponent->SetCBMovementMode(PreviousMovementMode);
}

void UDashState::BindInputActions()
{
	//InputComponent->BindAction("Dash", IE_Pressed, this, &UDashState::Dash);
}
