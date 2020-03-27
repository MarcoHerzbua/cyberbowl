// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CBCharacterMovementComponent.h"
#include "CyberbowlCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CYBERBOWL_API UCyberbowlCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:

		virtual void NativeInitializeAnimation() override;

		UFUNCTION(blueprintCallable)
		void UpdateAnimationProperties();

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		float movementSpeed;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		float movementTime;

		UPROPERTY(editAnywhere, BlueprintReadWrite)
		float maxMovementTime;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		float awayFromWallTime;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		float jumpVelocity;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bisInAir;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bisIsWallRidingCounterClockWise;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bisIsWallRidingClockWise;

		void setIsWallRidingCounterClockWise(bool bIsCounterClockWise);
		void setIsWallRidingClockWise(bool bisInWallRidingClockWise);

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		class APawn* pawn;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		class ACyberbowlCharacter* main;

private:
	float MaxCharacterSpeed;
	
};
