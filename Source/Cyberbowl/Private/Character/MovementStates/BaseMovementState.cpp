#include "Character/MovementStates/BaseMovementState.h"

void BaseMovementState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	MovementComponent = moveComponent;
}

void BaseMovementState::Activate()
{
}

void BaseMovementState::Deactivate()
{
}

void BaseMovementState::OnTick(float DeltaTime)
{
}
