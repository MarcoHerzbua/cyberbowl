#pragma once
#include "CoreMinimal.h"
#include "Character/MovementStates/IMovementState.h"
#include "BaseMovementState.generated.h"

/**
 *
 */
UCLASS()
class CYBERBOWL_API UBaseMovementState : public UObject
{
	GENERATED_BODY()
public:
	UBaseMovementState() {};
	virtual void InitializeState(class UCBCharacterMovementComponent* moveComponent);
	virtual void Activate();
	virtual void Deactivate();
	virtual void OnTick(float DeltaTime);

protected:
	UCBCharacterMovementComponent* MovementComponent;
};