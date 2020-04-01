// Fill out your copyright notice in the Description page of Project Settings.

#include "Cyberbowl/Public/Character/CyberbowlCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CyberbowlCharacter.h"


void UCyberbowlCharacterAnimInstance::NativeInitializeAnimation()
{
	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
		if (pawn)
		{
			main = Cast<ACyberbowlCharacter>(pawn);
		}
	}
	bisIsWallRidingCounterClockWise = false;
	bisIsWallRidingClockWise = false;
	awayFromWallTime = 0;
	movementTime = 0.6f;
	MaxCharacterSpeed = 0;

}

void UCyberbowlCharacterAnimInstance::UpdateAnimationProperties()
{
	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
	}

	if (pawn)
	{
		if (MaxCharacterSpeed == 0)
		{
			MaxCharacterSpeed = pawn->GetMovementComponent()->GetMaxSpeed();
		}
		
		FVector Speed = pawn->GetVelocity();
		FVector lateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		float movementSpeedOld = movementSpeed;
		movementSpeed = lateralSpeed.Size();
		jumpVelocity = Speed.Z;

		int movementSpeedPadding = 5.f;

		if ((int)movementSpeedOld- movementSpeedPadding >(int)movementSpeed || movementTime < maxMovementTime && movementTime > 0.f) //movementSpeed <= 375.f && movementTime < 4.0f))
		{
			movementTime-= GetWorld()->DeltaTimeSeconds;
		}
		
		else if (movementSpeed==0)
		{
			movementTime = 0.0f;
		}

		else if (movementSpeed >= 0)
		{
			movementTime = maxMovementTime;
		}
		
		bisInAir = pawn->GetMovementComponent()->IsFalling();

		if (bisIsWallRidingClockWise == false && bisIsWallRidingCounterClockWise == false)
		{
			awayFromWallTime += GetWorld()->DeltaTimeSeconds;
		}

	}
}

void UCyberbowlCharacterAnimInstance::setIsWallRidingCounterClockWise(bool bIsCounterClockWise)
{
	bisIsWallRidingCounterClockWise = bIsCounterClockWise;
	if (bIsCounterClockWise == true)
	{
		awayFromWallTime = 0;
	}
}

void UCyberbowlCharacterAnimInstance::setIsWallRidingClockWise(bool bisInWallRidingClockWise)
{
	bisIsWallRidingClockWise = bisInWallRidingClockWise;
	if (bisInWallRidingClockWise == true)
	{
		awayFromWallTime = 0;
	}
}

void UCyberbowlCharacterAnimInstance::setIsDashing(bool bIsDash)
{
	bIsDashing = bIsDash;
}

void UCyberbowlCharacterAnimInstance::setDashPlayRate(float playRate)
{
	dashPlayRate = 30*playRate;
}

