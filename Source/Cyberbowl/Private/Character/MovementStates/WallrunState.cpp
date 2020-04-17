#include "Character/MovementStates/WallrunState.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Character/CyberbowlCharacterAnimInstance.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

UWallrunState::UWallrunState()
{
}

void UWallrunState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	UBaseMovementState::InitializeState(moveComponent);

	DefaultGravityScale = MovementComponent->GravityScale;

}

void UWallrunState::Activate(ECBMovementMode previousMode)
{
	UBaseMovementState::Activate(previousMode);

	MovementComponent->GravityScale = 0.f;
	MovementComponent->Velocity.Z = 0;

	float length;
	MovementComponent->Velocity.ToDirectionAndLength(WallrunDirection, length);
	
	currInitializeAnimationFrames = 0;
}

void UWallrunState::Deactivate()
{
	UBaseMovementState::Deactivate();

	MovementComponent->GravityScale = DefaultGravityScale;

	MovementComponent->animinstance->setIsWallRidingClockWise(false);
	MovementComponent->animinstance->setIsWallRidingCounterClockWise(false);
	
}

void UWallrunState::OnTick(float DeltaTime)
{
	UBaseMovementState::OnTick(DeltaTime);

	float length;
	MovementComponent->Velocity.ToDirectionAndLength(WallrunDirection, length);
	WallrunDirection.Z = 0.f;

	if(bIsLaunching)
	{
		MovementComponent->Velocity = LaunchVector;
	}
	else
	{
		MovementComponent->Velocity = WallrunDirection * (MovementComponent->MaxAcceleration * MovementComponent->WallrunSpeedModifier);		
	}
	
	//More then one Tick is needed to finalize the WallRunDirection, therefore the frist 5 Ticks are used to correct the Animation.
	if (currInitializeAnimationFrames < initializeAnimationFrames)
	{
		FHitResult wallRunHitResult;
		EWallRunDirection hitPosition = HitDirection(wallRunHitResult);

		if (hitPosition == EWallRunDirection::WALLRUN_CLOCKWISE)
		{
			MovementComponent->animinstance->setIsWallRidingClockWise(true);
			MovementComponent->animinstance->setIsWallRidingCounterClockWise(false);
		}

		else
		{
			MovementComponent->animinstance->setIsWallRidingCounterClockWise(true);
			MovementComponent->animinstance->setIsWallRidingClockWise(false);
		}
		currInitializeAnimationFrames++;
	}
	
	FRotator wallrunDirRotator = WallrunDirection.Rotation();
	MovementComponent->GetCharacterOwner()->SetActorRotation(FRotator(0.f, wallrunDirRotator.Yaw, 0.f));
}

void UWallrunState::BindInputActions()
{
	InputComponent->BindAction("WallrunJump", IE_Pressed, this, &UWallrunState::LaunchCharacter);
}

void UWallrunState::LaunchCharacter()
{
	if(MovementComponent->GetCBMovementMode() != ECBMovementMode::CBMOVE_Wallrun)
	{
		return;
	}

	FHitResult wallRunHitResult;
	HitDirection(wallRunHitResult);
	FVector wallNormal = wallRunHitResult.Normal;

	//Calculate the angle between the normal pointing away from the wall and the current velocity
	float angleRadians = FMath::UnwindRadians(FMath::Atan2(wallNormal.Y, wallNormal.X) - FMath::Atan2(WallrunDirection.Y, WallrunDirection.X));
	float angle = FMath::RadiansToDegrees(angleRadians);
	angle = FMath::Clamp(angle, -MovementComponent->WallrunLaunchAngle, MovementComponent->WallrunLaunchAngle);

	FVector launchVec = WallrunDirection.RotateAngleAxis(angle, FVector::UpVector);

	launchVec *= MovementComponent->WallrunLaunchForce;
	launchVec.Z = MovementComponent->WallrunUpwardsLaunchForce;
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("x: %f, y: %f, z: %f"), launchVec.X, launchVec.Y, launchVec.Z));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("%f"), angleUp));

	bIsLaunching = true;
	LaunchVector = launchVec;
	MovementComponent->animinstance->setIsDashing(true);
	MovementComponent->GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, this, &UWallrunState::EndWallrun, MovementComponent->WallrunLaunchDuration);
	//MovementComponent->GetCharacterOwner()->LaunchCharacter(launchVec, true, true);

	//MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_Jump);
}

EWallRunDirection UWallrunState::HitDirection(FHitResult& hitResult)
{
	auto world = MovementComponent->GetWorld();
	auto actorLoc = MovementComponent->GetActorLocation();
	auto end1 = actorLoc + 200.f * WallrunDirection.RotateAngleAxis(90, FVector(0, 0, 1));
	auto end2 = actorLoc + 200.f * WallrunDirection.RotateAngleAxis(-90, FVector(0, 0, 1));

	FHitResult hit1, hit2;
	world->LineTraceSingleByProfile(hit1, actorLoc, end1, "WallrunTrace");
	world->LineTraceSingleByProfile(hit2, actorLoc, end2, "WallrunTrace");
	//DrawDebugLine(world, MovementComponent->GetActorLocation(), end1, FColor::Red, false, 5, 0, 3.f);
	//DrawDebugLine(world, MovementComponent->GetActorLocation(), end2, FColor::Blue, false, 5, 0, 3.f);
	FVector hitVec;
	if (hit1.bBlockingHit)
	{
		hitResult = hit1;
		UE_LOG(LogTemp, Warning, TEXT("WALLRUN_COUNTERCLOCKWISE"));
		return EWallRunDirection::WALLRUN_COUNTERCLOCKWISE;//WallrunDirection.RotateAngleAxis(-90, FVector(0, 0, 1));
	}
	else
	{
		hitResult = hit2;
		UE_LOG(LogTemp, Warning, TEXT("WALLRUN_CLOCKWISE"));
		return EWallRunDirection::WALLRUN_CLOCKWISE;//WallrunDirection.RotateAngleAxis(90, FVector(0, 0, 1));
	}
}

void UWallrunState::EndWallrun()
{
	bIsLaunching = false;
	LaunchVector = WallrunDirection = FVector::ZeroVector;
	MovementComponent->animinstance->setIsDashing(false);
	MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_Jump);
}

