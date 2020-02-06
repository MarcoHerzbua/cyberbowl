#include "Character/MovementStates/WallrunState.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

UWallrunState::UWallrunState()
{
}

void UWallrunState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	UBaseMovementState::InitializeState(moveComponent);

	DefaultGravityScale = MovementComponent->GravityScale;
}

void UWallrunState::Activate()
{
	UBaseMovementState::Activate();

	MovementComponent->GravityScale = 0.f;
}

void UWallrunState::Deactivate()
{
	UBaseMovementState::Deactivate();

	MovementComponent->GravityScale = DefaultGravityScale;
}

void UWallrunState::OnTick(float DeltaTime)
{
	if (!InputComponent)
	{
		InputComponent = MovementComponent->GetOwner()->InputComponent;
		//UE_LOG(LogClass, Error, TEXT("WallrunState no inputcomponent found"));
		//return;
		InputComponent->BindAction("Jump", IE_Pressed, this, &UWallrunState::LaunchCharacter);
	}

	MovementComponent->Velocity.Z = 0;

	float length;
	MovementComponent->Velocity.ToDirectionAndLength(WallrunDirection, length);

	MovementComponent->Velocity = WallrunDirection * (MovementComponent->MaxCustomMovementSpeed * MovementComponent->WallrunSpeedModifier);
}

void UWallrunState::LaunchCharacter()
{
	if(MovementComponent->GetCBMovementMode() != ECBMovementMode::CBMOVE_Wallrun)
	{
		return;
	}
	auto world = MovementComponent->GetWorld();
	auto actorLoc = MovementComponent->GetActorLocation();
	auto end1 = actorLoc + 200.f * WallrunDirection.RotateAngleAxis(90, FVector(0, 0, 1));
	auto end2 = actorLoc + 200.f * WallrunDirection.RotateAngleAxis(-90, FVector(0, 0, 1));

	FHitResult hit1, hit2;
	world->LineTraceSingleByProfile(hit1, actorLoc, end1, "WallrunTrace");
	world->LineTraceSingleByProfile(hit2, actorLoc, end2, "WallrunTrace");
	//DrawDebugLine(world, MovementComponent->GetActorLocation(), end1, FColor::Red, false, 5, 0, 3.f);
	//DrawDebugLine(world, MovementComponent->GetActorLocation(), end2, FColor::Blue, false, 5, 0, 3.f);
	FVector launchVec;
	if(hit1.bBlockingHit)
	{
		launchVec = hit1.Normal;//WallrunDirection.RotateAngleAxis(-90, FVector(0, 0, 1));
	}
	else
	{
		launchVec = hit2.Normal;//WallrunDirection.RotateAngleAxis(90, FVector(0, 0, 1));
	}

	launchVec = launchVec.RotateAngleAxis(MovementComponent->WallrunLaunchAngle, FVector(0, 0, 1));
	launchVec *= MovementComponent->WallrunLaunchForce;
	launchVec.Z = MovementComponent->WallrunLaunchForce;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("x: %f, y: %f, z: %f"), launchVec.X, launchVec.Y, launchVec.Z));
	
	MovementComponent->AddImpulse(launchVec, true);
	//auto ownerAsCharacter = Cast<ACharacter>(MovementComponent->GetOwner());
	//ownerAsCharacter->LaunchCharacter(launchVec, true, true);

	MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_None);
}
