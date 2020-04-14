#pragma once

#include "CoreMinimal.h"
#include "Character/MovementStates/BaseMovementState.h"
//#include "K2Node_FunctionResult.h"
#include "WallrunState.generated.h"


/**
 *
 */
UENUM(BlueprintType)
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
	const int initializeAnimationFrames = 5;
	int currInitializeAnimationFrames;

	void BindInputActions() override;

	UFUNCTION()
	void LaunchCharacter();
	UFUNCTION()
	EWallRunDirection HitDirection(FHitResult& hitResult);
	
};