#include "Character/MovementStates/WallrunState.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Character/CyberbowlCharacterAnimInstance.h"
#include "Engine/Engine.h"

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

	float length;
	MovementComponent->Velocity.ToDirectionAndLength(WallrunDirection, length);
	
	FHitResult wallRunHitResult;
	EWallRunDirection hitPosition = HitDirection(wallRunHitResult);

	if (hitPosition == EWallRunDirection::WALLRUN_CLOCKWISE)
	{
		MovementComponent->animinstance->setIsWallRidingClockWise(true);
	}
	
	else
	{
		MovementComponent->animinstance->setIsWallRidingCounterClockWise(true);
	}
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

	MovementComponent->Velocity.Z = 0;

	float length;
	MovementComponent->Velocity.ToDirectionAndLength(WallrunDirection, length);

	MovementComponent->Velocity = WallrunDirection * (MovementComponent->MaxWalkSpeed * MovementComponent->WallrunSpeedModifier);
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
	FVector launchVec = wallRunHitResult.Normal;
	launchVec = launchVec.RotateAngleAxis(MovementComponent->WallrunLaunchAngle, FVector(0, 0, 1));
	launchVec *= MovementComponent->WallrunLaunchForce;
	launchVec.Z = MovementComponent->WallrunLaunchForce;
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("x: %f, y: %f, z: %f"), launchVec.X, launchVec.Y, launchVec.Z));
	
	MovementComponent->AddImpulse(launchVec, true);
	//auto ownerAsCharacter = Cast<ACharacter>(MovementComponent->GetOwner());
	//ownerAsCharacter->LaunchCharacter(launchVec, true, true);

	MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_Running);
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
	DrawDebugLine(world, MovementComponent->GetActorLocation(), end1, FColor::Red, false, 5, 0, 3.f);
	DrawDebugLine(world, MovementComponent->GetActorLocation(), end2, FColor::Blue, false, 5, 0, 3.f);
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

