#include "Character/MovementStates/BaseMovementState.h"
#include "Character/CBCharacterMovementComponent.h"

void UBaseMovementState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	MovementComponent = moveComponent;
}

void UBaseMovementState::Activate()
{
	if (!InputComponent)
	{
		InputComponent = MovementComponent->GetOwner()->InputComponent;
		BindInputActions();
	}
}

void UBaseMovementState::Deactivate()
{
}

void UBaseMovementState::OnTick(float DeltaTime)
{
	//if(!InputComponent)
	//{
	//	InputComponent = MovementComponent->GetOwner()->InputComponent;
	//}
}

void UBaseMovementState::BindInputActions()
{
}
