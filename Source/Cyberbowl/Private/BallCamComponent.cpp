// Fill out your copyright notice in the Description page of Project Settings.


#include "BallCamComponent.h"

#include "Character/CyberbowlCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include <string>
#include "Camera/CameraComponent.h"

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

void UBallCamComponent::FocusBall(float deltaTime)
{
	FRotator lookAtBallRot = UKismetMathLibrary::FindLookAtRotation(PlayBall->GetComponentLocation(), CameraBoom->GetComponentLocation());
	float lookAtPitch = lookAtBallRot.Pitch * (-1);
	float lookAtYaw = lookAtBallRot.Yaw + 180;

	FMinimalViewInfo viewInfo;
	Cast<ACyberbowlCharacter>(GetOwner())->GetFollowCamera()->GetCameraView(deltaTime, viewInfo);
	float cameraPitch = viewInfo.Rotation.Pitch;

	float smoothedPitch = FMath::FInterpTo(cameraPitch, lookAtPitch, deltaTime, 2.f);

	if (lookAtPitch <= 25.f)
	{
		smoothedPitch = FMath::FInterpTo(cameraPitch, 0, deltaTime, 1.5f);
	}

	CameraBoom->SetWorldRotation(FRotator(smoothedPitch, lookAtYaw, 0));
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
