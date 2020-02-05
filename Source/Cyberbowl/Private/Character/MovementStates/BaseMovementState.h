#pragma once
#include "CoreMinimal.h"
#include "Character/MovementStates/IMovementState.h"

/**
 *
 */
class CYBERBOWL_API BaseMovementState
{
public:
	virtual void InitializeState(class UCBCharacterMovementComponent* moveComponent);
	virtual void Activate();
	virtual void Deactivate();
	virtual void OnTick(float DeltaTime);

protected:
	UCBCharacterMovementComponent* MovementComponent;
};