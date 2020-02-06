#include "Character/MovementStates/BaseMovementState.h"

void UBaseMovementState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	MovementComponent = moveComponent;
}

void UBaseMovementState::Activate()
{
}

void UBaseMovementState::Deactivate()
{
}

void UBaseMovementState::OnTick(float DeltaTime)
{
}
