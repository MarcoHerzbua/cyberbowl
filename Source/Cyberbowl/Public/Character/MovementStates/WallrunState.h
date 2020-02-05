#pragma once

#include "CoreMinimal.h"
#include "Character/MovementStates/BaseMovementState.h"


/**
 *
 */
class CYBERBOWL_API WallrunState : public BaseMovementState
{
public:
	WallrunState();

	void InitializeState(class UCBCharacterMovementComponent* moveComponent) override;
	void Activate() override;
	void Deactivate() override;
	void OnTick(float DeltaTime) override;

protected:
	class UInputComponent* InputComponent;
	float DefaultGravityScale;
	FVector WallrunDirection;

	void LaunchCharacter();
};