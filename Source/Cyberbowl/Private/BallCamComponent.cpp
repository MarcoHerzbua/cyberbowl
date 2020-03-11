// Fill out your copyright notice in the Description page of Project Settings.


#include "BallCamComponent.h"

#include "Cyberbowl/CyberbowlCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Points.h"

UBallCamComponent::UBallCamComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBallCamComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraBoom = Cast<ACyberbowlCharacter>(GetOwner())->GetCameraBoom();
	PlayBall = Cast<UStaticMeshComponent>(Cast<APoints>(UGameplayStatics::GetGameMode(this))->Ball->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}


void UBallCamComponent::FocusBall()
{
	FVector ballWorldLocation = PlayBall->GetComponentLocation();
	FVector cameraBoomWorldLocation = CameraBoom->GetComponentLocation();
	
	FRotator lookAtBallRot = UKismetMathLibrary::FindLookAtRotation(ballWorldLocation, cameraBoomWorldLocation);
	float newPitch = 360 - lookAtBallRot.Pitch;
	float newYaw = lookAtBallRot.Yaw - 180;
	CameraBoom->SetWorldRotation(FRotator(newPitch, newYaw, lookAtBallRot.Roll));
}

void UBallCamComponent::ToggleBallCam()
{
	bShouldFollowBall = !bShouldFollowBall;
	CameraBoom->bUsePawnControlRotation = !bShouldFollowBall;

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

	if (bShouldFollowBall)
	{
		FocusBall();
	}
}
