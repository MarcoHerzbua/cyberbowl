// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MovementStates/BaseMovementState.h"
#include "CBCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECBMovementMode : uint8
{
	CBMOVE_None UMETA(DisplayName="None"),
	CBMOVE_Wallrun UMETA(DisplayName="Wallrun")
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
	
	UFUNCTION(BlueprintCallable)
	void SetCBMovementMode(ECBMovementMode mode);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY()
	ECBMovementMode CBMovementMode;

	TMap<ECBMovementMode, BaseMovementState*> MovementStates;
	
	virtual void BeginPlay() override;

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


