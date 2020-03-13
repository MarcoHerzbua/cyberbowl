#include "Character/MovementStates/DashState.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"


void UDashState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	UBaseMovementState::InitializeState(moveComponent);
	DefaultGravityScale = MovementComponent->GravityScale;
}

void UDashState::Activate()
{
	UBaseMovementState::Activate();
	
	InitialVelocity = MovementComponent->Velocity;
	MovementComponent->GravityScale = 0.f;
	MovementComponent->GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UDashState::StopDash, MovementComponent->DashDuration);
}

void UDashState::Deactivate()
{
	UBaseMovementState::Deactivate();

	MovementComponent->GravityScale = DefaultGravityScale;
	MovementComponent->GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
}

void UDashState::OnTick(float DeltaTime)
{
	auto dashDirection = MovementComponent->GetCurrentAcceleration().GetSafeNormal();

	MovementComponent->Velocity = dashDirection * MovementComponent->DashForce;
}

void UDashState::StopDash()
{
	MovementComponent->StopMovementImmediately();
	MovementComponent->Velocity = InitialVelocity;
	MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_DoubleJump);
}

void UDashState::BindInputActions()
{
	//InputComponent->BindAction("Dash", IE_Pressed, this, &UDashState::Dash);
}
