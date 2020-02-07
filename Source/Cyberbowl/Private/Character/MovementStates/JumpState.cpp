#include "Character/MovementStates/JumpState.h"

void UJumpState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	UBaseMovementState::InitializeState(moveComponent);
}

void UJumpState::Activate()
{
	UBaseMovementState::Activate();

}

void UJumpState::Deactivate()
{
	UBaseMovementState::Deactivate();
}

void UJumpState::OnTick(float DeltaTime)
{
	UBaseMovementState::OnTick(DeltaTime);

}
