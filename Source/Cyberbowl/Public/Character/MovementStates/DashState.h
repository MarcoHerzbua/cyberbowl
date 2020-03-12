#pragma once

#include "CoreMinimal.h"
#include "Character/MovementStates/BaseMovementState.h"
#include "DashState.generated.h"


/**
 *
 */
UCLASS()
class CYBERBOWL_API UDashState : public UBaseMovementState
{
	GENERATED_BODY()
public:
	UDashState() {}

	void InitializeState(class UCBCharacterMovementComponent* moveComponent) override;
	void Activate() override;
	void Deactivate() override;
	void OnTick(float DeltaTime) override;

protected:
	float DefaultGravityScale;
	FVector InitialVelocity;
	
	void StopDash();

	UPROPERTY()
	FTimerHandle DashTimerHandle;
	
	void BindInputActions() override;
};

