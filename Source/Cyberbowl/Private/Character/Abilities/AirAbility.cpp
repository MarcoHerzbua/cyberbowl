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
		
		if (UKismetMathLibrary::Abs(FVector::Distance(ball->GetActorLocation(), ballPulledAttachComponent->GetComponentLocation())) <= 100.f)
		{
			ball->AttachToComponent(ballPulledAttachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		else
		{
			const auto ballTarget = UKismetMathLibrary::VInterpTo(ball->GetActorLocation(), ballPulledAttachComponent->GetComponentLocation(), DeltaTime, succSpeed);
			ball->SetActorLocation(ballTarget);
		}

		const FRotator cameraLookAt = FRotator(character->GetCameraBoom()->GetTargetRotation().Pitch * (-1), ballLocationSpringArm->GetComponentRotation().Yaw, 0);
		ballLocationSpringArm->SetWorldRotation(cameraLookAt);
	}
}

void UAirAbility::Fire()
{
	FVector cylinderEnd = GetOwner()->GetActorLocation();
	cylinderEnd.Z += 2000.f;
	DrawDebugCylinder(GetWorld(), GetOwner()->GetActorLocation(), cylinderEnd, grabRadiusMeters, 32, FColor::Red, false, grabDurationSeconds, 0, 5.f);

	const float distance = FVector2D::Distance(FVector2D(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y),FVector2D(ball->GetActorLocation().X, ball->GetActorLocation().Y));
	
	if (distance <= grabRadiusMeters)
	{
		bIsInGrabMode = true;

		// Used for character control with right stick
		character->bTurretMode = true;
		
		//ball->StopBall();
		ball->BallStaticMesh->SetEnableGravity(false);
		movementComp->DisableMovement();
		
		// Disable BallCam if active
		auto ballCamComp = Cast<UBallCamComponent>(Cast<ACyberbowlCharacter>(GetOwner())->GetComponentByClass(UBallCamComponent::StaticClass()));
		ballCamComp->DoNotFollow();

		GetWorld()->GetTimerManager().SetTimer(GrabModeDurationHandle, this, &UAirAbility::ExitGrabMode, grabDurationSeconds, false);

		const auto cameraLookAtRotation = FRotator(0.f, character->GetCameraBoom()->GetTargetRotation().Yaw, 0.f);
		character->SetActorRotation(cameraLookAtRotation);
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
	ball->BallStaticMesh->SetEnableGravity(true);

	character->bTurretMode = false;
	character->GetCameraBoom()->bUsePawnControlRotation = true;
	movementComp->SetMovementMode(EMovementMode::MOVE_Walking);
}
