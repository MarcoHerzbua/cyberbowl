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
}

void UCyberbowlCharacterAnimInstance::UpdateAnimationProperties()
{
	if (pawn == nullptr)
	{
		pawn = TryGetPawnOwner();
	}

	if (pawn)
	{
		FVector Speed = pawn->GetVelocity();
		FVector lateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		movementSpeed = lateralSpeed.Size();
		jumpVelocity = Speed.Z;

		bisInAir = pawn->GetMovementComponent()->IsFalling();

	}
}

