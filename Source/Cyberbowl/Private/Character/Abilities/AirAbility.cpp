// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AirAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include <string>
#include "DrawDebugHelpers.h"
#include "Actors/PlayBall.h"
#include "Character/CyberbowlCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Actors/PlayBall.h"
#include "PlayerController/ThirdPersonPlayerController.h"
#include "Character/BallCamComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"

void UAirAbility::BeginPlay()
{
	Super::BeginPlay();
	
	character = Cast<ACyberbowlCharacter>(GetOwner());
	movementComp = Cast<UCharacterMovementComponent>(character->GetMovementComponent());

	ball = Cast<APlayBall>(Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->Ball);
	ballPulledAttachComponent = Cast<USceneComponent>(character->GetComponentsByTag(USceneComponent::StaticClass(), "TornadoBallLocation").Last());
}

void UAirAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInGrabMode)
	{
		auto ballLocationSpringArm = Cast<USpringArmComponent>(character->GetComponentsByTag(USpringArmComponent::StaticClass(), "BallLocationArm").Last());
		FRotator cameraLookAt = FRotator(character->GetCameraBoom()->GetTargetRotation().Pitch * (-1), ballLocationSpringArm->GetComponentRotation().Yaw, 0);
		ballLocationSpringArm->SetWorldRotation(cameraLookAt);
		ball->SetActorLocation(ballPulledAttachComponent->GetComponentLocation());	
	}
}

void UAirAbility::Fire()
{
	FVector cylinderEnd = GetOwner()->GetActorLocation();
	cylinderEnd.Z += 2000.f;
	DrawDebugCylinder(GetWorld(), GetOwner()->GetActorLocation(), cylinderEnd, 1000.f, 32, FColor::Red, false, 3.f, 0, 5.f);

	const float distance = FVector::Distance(GetOwner()->GetActorLocation(), ball->GetActorLocation());
	
	if (distance <= radiusMeters)
	{
		bIsInGrabMode = true;
		character->bTurretMode = true;
		
		auto ballCamComp = Cast<UBallCamComponent>(Cast<ACyberbowlCharacter>(GetOwner())->GetComponentByClass(UBallCamComponent::StaticClass()));
		ballCamComp->DoNotFollow();
		
		ball->StopBall();

		GetWorld()->GetTimerManager().SetTimer(GrabModeDurationHandle, this, &UAirAbility::ExitGrabMode, grabDurationSeconds, false);

		// ToDo: Set ball in center of camera, considering the scene component on the character

		/*const auto cameraManager = UGameplayStatics::GetPlayerCameraManager(this, UGameplayStatics::GetPlayerControllerID(Cast<AThirdPersonPlayerController>(Cast<ACyberbowlCharacter>(GetOwner())->Controller)));
		FVector cameraForwardVector = cameraManager->GetActorForwardVector();
		
		FVector cameraLookAtLocation;
		ball->SetActorLocation(cameraLookAtLocation);*/
		
		// ToDo: Rotate the  character towards the ball
		
		auto lookAtRotation = UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation(), ball->GetActorLocation());
		auto newRotation = FRotator(0, lookAtRotation.Yaw, 0);
		character->SetActorRotation(newRotation);
		Cast<AThirdPersonPlayerController>(character->GetController())->SetControlRotation(newRotation);
		
		// ToDo: Disable movement
		
		movementComp->DisableMovement();
		
		// ToDo: Rotate character + ball with right stick

		//character->GetViewRotation().Pitch
		
		// ToDo: Break GrabMode when ball is affected by Push
	}
}

void UAirAbility::ConvertMetersToUnrealUnits()
{
	// Conversion from meters to cm, as unreal functions generally output centimeters, but meters is easier for game design tweaks
	radiusMeters *= 100.f;
}

void UAirAbility::ExitGrabMode()
{
	bIsInGrabMode = false;
	ball->ResumeBall();
	movementComp->SetMovementMode(EMovementMode::MOVE_Walking);
	character->bTurretMode = false;
	character->GetCameraBoom()->bUsePawnControlRotation = true;
}
