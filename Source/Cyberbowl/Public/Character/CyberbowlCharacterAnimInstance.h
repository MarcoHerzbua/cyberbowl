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
		bool bIsDoubleJumping;

		UPROPERTY(editAnywhere, BlueprintReadWrite)
		float doubleJumpPlayRate = 1.0f;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bisIsWallRidingCounterClockWise;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bisIsWallRidingClockWise;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bIsDashing;
	
		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bIsBooping;

		UPROPERTY(editAnywhere, BlueprintReadWrite)
		float boopPlayRate = 1.0f;

		UPROPERTY(editDefaultsOnly, blueprintReadWrite, Category = "Animation")
		class UAnimMontage* cyberbowlMonatage;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		float dashPlayRate = 1.f;

		void SetIsWallRidingCounterClockWise(bool bIsCounterClockWise);
		void SetIsWallRidingClockWise(bool bisInWallRidingClockWise);
		void SetIsDashing(bool bDash);
		void SetDashPlayRate(float playRate);
		void SetIsBooping(bool bBooping);
		void SetBoopPlayRate(float playRate);
		void SetIsDoubleJumping(bool bDoubleJump);
		void SetDoubleJumpPlayRate(float playRate);

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		class APawn* pawn;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		class ACyberbowlCharacter* main;

private:
	float MaxCharacterSpeed;
	
};
