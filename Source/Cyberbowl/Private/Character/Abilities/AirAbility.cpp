// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AirAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include "DrawDebugHelpers.h"
#include "Actors/PlayBall.h"
#include "Character/CyberbowlCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
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
	ballLocationSpringArm = Cast<USpringArmComponent>(character->GetComponentsByTag(USpringArmComponent::StaticClass(), "BallLocationArm").Last());
	ballPulledAttachComponent = Cast<USceneComponent>(character->GetComponentsByTag(USceneComponent::StaticClass(), "TornadoBallLocation").Last());

	ball->OnBallBooped.AddDynamic(this, &UAirAbility::ExitGrabMode);
}

void UAirAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInGrabMode)
	{		
		FRotator cameraLookAt = FRotator(character->GetCameraBoom()->GetTargetRotation().Pitch * (-1), ballLocationSpringArm->GetComponentRotation().Yaw, 0);

		if (UKismetMathLibrary::Abs(cameraLookAt.Pitch) >= 180)
		{
			cameraLookAt.Pitch = 0.0f;
		}
		
		ballLocationSpringArm->SetWorldRotation(cameraLookAt);
	}
}

void UAirAbility::Fire()
{
	FVector cylinderEnd = GetOwner()->GetActorLocation();
	cylinderEnd.Z += 2000.f;
	DrawDebugCylinder(GetWorld(), GetOwner()->GetActorLocation(), cylinderEnd, 1000.f, 32, FColor::Red, false, 3.f, 0, 5.f);

	const float distance = FVector2D::Distance(FVector2D(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y),FVector2D(ball->GetActorLocation().X, ball->GetActorLocation().Y));
	
	if (distance <= grabRadiusMeters)
	{
		bIsInGrabMode = true;
		character->bTurretMode = true;
		ball->StopBall();
		ball->AttachToComponent(ballPulledAttachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		movementComp->DisableMovement();
		
		// Disable BallCam if active
		auto ballCamComp = Cast<UBallCamComponent>(Cast<ACyberbowlCharacter>(GetOwner())->GetComponentByClass(UBallCamComponent::StaticClass()));
		ballCamComp->DoNotFollow();

		GetWorld()->GetTimerManager().SetTimer(GrabModeDurationHandle, this, &UAirAbility::ExitGrabMode, grabDurationSeconds, false);
		
		auto lookAtRotation = UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation(), ball->GetActorLocation());
		auto newRotation = FRotator(0, lookAtRotation.Yaw, 0);
		character->SetActorRotation(newRotation);
		Cast<AThirdPersonPlayerController>(character->GetController())->SetControlRotation(newRotation);		
	}
}

void UAirAbility::ConvertMetersToUnrealUnits()
{
	// Conversion from meters to cm, as unreal functions generally output centimeters, but meters is easier for game design tweaks
	grabRadiusMeters *= 100.f;
}

void UAirAbility::ExitGrabMode()
{
	if (!bIsInGrabMode)
	{
		return;
	}
	
	bIsInGrabMode = false;
	ball->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ball->ResumeBall();
	movementComp->SetMovementMode(EMovementMode::MOVE_Walking);
	character->bTurretMode = false;
	character->GetCameraBoom()->bUsePawnControlRotation = true;
}
