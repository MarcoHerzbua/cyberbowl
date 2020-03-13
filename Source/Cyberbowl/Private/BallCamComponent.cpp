// Fill out your copyright notice in the Description page of Project Settings.


#include "BallCamComponent.h"

#include "Character/CyberbowlCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include <string>

UBallCamComponent::UBallCamComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBallCamComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraBoom = Cast<ACyberbowlCharacter>(GetOwner())->GetCameraBoom();
	PlayBall = Cast<UStaticMeshComponent>(Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->Ball->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

void UBallCamComponent::Lerp(float from, float to, float seconds)
{
	
}


void UBallCamComponent::FocusBall(float deltaTime)
{
	FVector ballWorldLocation = PlayBall->GetComponentLocation();
	FVector cameraBoomWorldLocation = CameraBoom->GetComponentLocation();
	
	FRotator lookAtBallRot = UKismetMathLibrary::FindLookAtRotation(ballWorldLocation, cameraBoomWorldLocation);
	float newPitch = lookAtBallRot.Pitch * (-1);
	float newYaw = lookAtBallRot.Yaw - 180;
	CameraBoom->SetWorldRotation(FRotator(CameraBoom->GetComponentRotation().Pitch, newYaw, 0));

	float smoothedPitch;
	
	if (newPitch > 25.f)
	{
		smoothedPitch = FMath::FInterpTo(CameraBoom->GetComponentRotation().Pitch, newPitch, deltaTime, 2.f);
	}
	else
	{
		smoothedPitch = FMath::FInterpTo(CameraBoom->GetComponentRotation().Pitch, 0, deltaTime, 1.5f);		
	}

	CameraBoom->SetWorldRotation(FRotator(smoothedPitch, newYaw, 0));
}

void UBallCamComponent::ToggleBallCam()
{
	bShouldFollowBall = !bShouldFollowBall;
	CameraBoom->bUsePawnControlRotation = !bShouldFollowBall;

	CameraBoom->SetWorldRotation(FRotator(0, 0, 0));

	// This ensures the camera stays the way it was when exiting the ball cam
	if (!bShouldFollowBall)
	{
		FRotator lookAtBallRot = UKismetMathLibrary::FindLookAtRotation(CameraBoom->GetComponentLocation(), PlayBall->GetComponentLocation());
		
		Cast<APlayerController>(Cast<ACyberbowlCharacter>(GetOwner())->GetController())->SetControlRotation(lookAtBallRot);
	}
}

// Called every frame
void UBallCamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bShouldFollowBall)
	{
		return;
	}

	FocusBall(DeltaTime);
}
