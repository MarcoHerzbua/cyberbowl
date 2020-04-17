// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/CooldownComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MovementStates/BaseMovementState.h"
#include "CBCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECBMovementMode : uint8
{
	CBMOVE_Running UMETA(DisplayName="Running"),
	CBMOVE_Wallrun UMETA(DisplayName="Wallrun"),
	CBMOVE_Jump UMETA(DisplayName="Jump"),
	CBMOVE_DoubleJump UMETA(DisplayName="DoubleJump"),
	CBMOVE_Dash UMETA(DisplayName="Dash")
};

/**
 * 
 */
UCLASS()
class CYBERBOWL_API UCBCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	//modifies the speed of the wallrun in relation to the maxMovementSpeed
	UPROPERTY(EditAnywhere, Category = "Wallrun Params")
	float WallrunSpeedModifier = 1.5f;

	//modifies the force by which the character gets launched away from the wall when jumping during wallrun
	UPROPERTY(EditAnywhere, Category = "Wallrun Params")
	float WallrunLaunchForce = 700.f;
	
	//modifies the force by which the character gets launched UPWARDS from the wall when jumping during wallrun
	UPROPERTY(EditAnywhere, Category = "Wallrun Params")
	float WallrunUpwardsLaunchForce = 3000.f;
	
	//modifies how long the character is launched (how long the Launch Forces are applied to the char, similar to Dash)
	UPROPERTY(EditAnywhere, Category = "Wallrun Params")
	float WallrunLaunchDuration = 0.1f;
	
	//modifies the angle in which the character get launched away from the wall when jumping during wallrun
	UPROPERTY(EditAnywhere, Category = "Wallrun Params")
	float WallrunLaunchAngle = 45.f;

	UPROPERTY(EditAnywhere, Category = "Dash Params")
	float DashDuration = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Dash Params")
	float DashForce = 10000.f;

	/*
	 * 	This is the Range (in degrees) in which the character does not keep momentum after dashing, takes the View of the camera as starting point 
	 *	Pressing forward on the LeftStick -> 0 degrees
	 *	Pressing backwards on the LeftStick -> 180 degrees
	 *	Pressing Right on the LeftStick -> 90 degrees
	 *	(No need to worry about negative values, we take the absolute of the angle)
	 */
	UPROPERTY(EditAnywhere, Category = "Dash Params")
	FFloatRange DashMomentumStopRange = TRange<float>(45.f, 135.f);

	//UFUNCTION(BlueprintCallable, Category = "Cyberbowl Movement")
	//void Jump();
	
	UFUNCTION(BlueprintCallable)
	void SetCBMovementMode(ECBMovementMode mode);

	UFUNCTION(BlueprintCallable)
	ECBMovementMode GetCBMovementMode() { return CBMovementMode; }

	UFUNCTION(BlueprintCallable)
	UBaseMovementState* GetCBMovementState() { return MovementStates.Contains(GetCBMovementMode()) ? MovementStates[GetCBMovementMode()] : nullptr; }

	FVector GetCharacterTransform() { return GetOwner()->GetActorLocation(); };

	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UCyberbowlCharacterAnimInstance* animinstance;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	ECBMovementMode CBMovementMode;

	UPROPERTY()
	TMap<ECBMovementMode, UBaseMovementState*> MovementStates;

	//UPROPERTY()
	//UCooldownComponent* CooldownComponent;
	//
	virtual void BeginPlay() override;

	void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
/*
 *
 *
 * EXPERIMENTAL MOVEMENT
 * Experiment trying to add custom Movementmodes to the existing MovementMode System of Unreal
 * Is quite complex and makes my brain go ouchie
 *
 */
	void PhysCustom(float deltaTime, int32 Iterations) override;

	void PhysWallrun(float deltaTime, int32 Iterations);
/*
 *
 *
 * 
 */
};


