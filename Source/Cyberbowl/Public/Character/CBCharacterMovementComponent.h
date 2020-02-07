// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	CBMOVE_DoubleJump UMETA(DisplayName="DoubleJump")
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
	UPROPERTY(EditAnywhere)
	float WallrunSpeedModifier = 1.5f;

	//modifies the force by which the character gets launched away from the wall when jumping during wallrun
	UPROPERTY(EditAnywhere)
	float WallrunLaunchForce = 700.f;
	
	//modifies the angle in which the character get launched away from the wall when jumping during wallrun
	UPROPERTY(EditAnywhere)
	float WallrunLaunchAngle = 45.f;
	
	UFUNCTION(BlueprintCallable)
	void SetCBMovementMode(ECBMovementMode mode);

	UFUNCTION(BlueprintCallable)
	ECBMovementMode GetCBMovementMode() { return CBMovementMode; }
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY(BlueprintReadOnly)
	ECBMovementMode CBMovementMode;

	UPROPERTY()
	TMap<ECBMovementMode, UBaseMovementState*> MovementStates;
	
	virtual void BeginPlay() override;

	void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
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


