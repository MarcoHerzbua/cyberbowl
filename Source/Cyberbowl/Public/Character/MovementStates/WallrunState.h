#pragma once

#include "CoreMinimal.h"
#include "Character/MovementStates/BaseMovementState.h"
#include "K2Node_FunctionResult.h"
#include "WallrunState.generated.h"


/**
 *
 */

enum class EWallRunDirection : uint8
{
	WALLRUN_CLOCKWISE,
	WALLRUN_COUNTERCLOCKWISE
};


UCLASS()
class CYBERBOWL_API UWallrunState : public UBaseMovementState
{
	GENERATED_BODY()
public:
	UWallrunState();

	void InitializeState(class UCBCharacterMovementComponent* moveComponent) override;
	void Activate(ECBMovementMode previousMode) override;
	void Deactivate() override;
	void OnTick(float DeltaTime) override;

protected:
	float DefaultGravityScale;
	FVector WallrunDirection;

	void BindInputActions() override;

	UFUNCTION()
	void LaunchCharacter();
	EWallRunDirection HitDirection(FHitResult& hitResult);
	
};