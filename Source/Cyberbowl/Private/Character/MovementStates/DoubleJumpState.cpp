#include "Character/MovementStates/DoubleJumpState.h"

void UDoubleJumpState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	UBaseMovementState::InitializeState(moveComponent);
}

void UDoubleJumpState::Activate()
{
	UBaseMovementState::Activate();
}

void UDoubleJumpState::Deactivate()
{
	UBaseMovementState::Deactivate();
}

void UDoubleJumpState::OnTick(float DeltaTime)
{
	UBaseMovementState::OnTick(DeltaTime);
}
