#include "Character/MovementStates/WallrunState.h"
#include "Character/CBCharacterMovementComponent.h"

WallrunState::WallrunState()
{
}

void WallrunState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	BaseMovementState::InitializeState(moveComponent);

	DefaultGravityScale = MovementComponent->GravityScale;
}

void WallrunState::Activate()
{
	BaseMovementState::Activate();

	MovementComponent->GravityScale = 0.f;
}

void WallrunState::Deactivate()
{
	BaseMovementState::Deactivate();

	MovementComponent->GravityScale = DefaultGravityScale;
}

void WallrunState::OnTick(float DeltaTime)
{
	MovementComponent->Velocity.Z = 0;
}
