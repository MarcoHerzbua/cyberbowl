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

void UAirAbility::BeginPlay()
{
	Super::BeginPlay();
	
	ball = Cast<APlayBall>(Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->Ball);
	ballPulledAttachComponent = Cast<USceneComponent>(GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), "TornadoBallLocation").Last());
}

void UAirAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInGrabMode)
	{
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
		ball->StopBall();
		bIsInGrabMode = true;
		GetWorld()->GetTimerManager().SetTimer(GrabModeDurationHandle, this, &UAirAbility::SetBallUngrabbed, grabDurationSeconds, false);
	}
	//Cast<ACyberbowlCharacter>(GetOwner())->GetMovementComponent()->ToggleActive();
}

void UAirAbility::ConvertMetersToUnrealUnits()
{
	// Conversion from meters to cm, as unreal functions generally output centimeters, but meters is easier for game design tweaks
	radiusMeters *= 100.f;
}

void UAirAbility::SetBallUngrabbed()
{
	bIsInGrabMode = false;
	ball->ResumeBall();
}
