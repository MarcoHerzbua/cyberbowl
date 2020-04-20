#pragma once

#include "CoreMinimal.h"
#include "Character/MovementStates/BaseMovementState.h"
//#include "K2Node_FunctionResult.h"
#include "WallrunState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWallrunFinish, float, timeOnWall, bool, launchedAway);

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

	UFUNCTION(BlueprintCallable)
	bool IsLaunching() { return bIsLaunching; }

	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnWallrunFinish OnWallrunFinish;
	
protected:
	float DefaultGravityScale;
	FVector WallrunDirection;
	FVector LaunchVector;
	const int initializeAnimationFrames = 5;
	int currInitializeAnimationFrames;
	bool bIsLaunching = false;

	UPROPERTY()
	FTimerHandle LaunchTimerHandle;

	void BindInputActions() override;

	UFUNCTION()
	void LaunchCharacter();
	UFUNCTION()
	EWallRunDirection HitDirection(FHitResult& hitResult);
	UFUNCTION()
	void EndWallrun();

	float timeOnWall;
};