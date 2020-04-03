// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/EarthAbility.h"
#include "Character/CyberbowlCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Abilities/CooldownComponent.h"
#include "PlayerController/ThirdPersonPlayerController.h"

#include "Character/Abilities/CooldownComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

void UEarthAbility::Fire()
{
	if(!bValidTarget)
	{
		SetAbilityState(EAbilityState::ABILITY_TARGETING);
		return;
	}

	if(!GetWorld()->GetTimerManager().IsTimerActive(LeapTimerHandle))
	{
		LeapStart = GetOwner()->GetActorLocation();
		GetWorld()->GetTimerManager().SetTimer(LeapTimerHandle, this, &UEarthAbility::EndLeap, LeapDuration);
	}
	else
	{
		DoLeap();
	}
}

void UEarthAbility::Targeting()
{
	UWorld* world = GetWorld();
	auto ownerAsPawn = Cast<APawn>(GetOwner());
	FVector actorLoc = GetOwner()->GetActorLocation();
	FVector camViewDir = ownerAsPawn->GetControlRotation().Vector();
	FVector end = actorLoc + camViewDir * MaxTargetDistance;

	FHitResult hitResult;
	world->LineTraceSingleByProfile(hitResult, actorLoc, end, "AbilityTrace");

	FRotator camViewRotator = ownerAsPawn->GetControlRotation();
	//Set Pitch to zero cause we are only interested in the Yaw (Roll is always 0)
	camViewRotator.Pitch = 0.f;

	bValidTarget = true;
	if (hitResult.bBlockingHit)
	{
		LeapTarget = hitResult.ImpactPoint;

		//Edge Case when player aims at the Wall
		if(hitResult.GetComponent()->GetCollisionProfileName() == "StadiumWall")
		{
			//get a point a certain distant away from the wall
			FVector hitPointWithOffset = hitResult.ImpactPoint + hitResult.ImpactNormal * TargetIndicatorRadius;

			FHitResult floorTrace;
			world->LineTraceSingleByProfile(floorTrace, hitPointWithOffset, hitPointWithOffset + FVector::DownVector * 10000.f, "AbilityTrace");

			if (floorTrace.bBlockingHit)
			{
				float distanceToActor = (floorTrace.ImpactPoint - actorLoc).Size();
				if (distanceToActor < MinTargetDistance)
				{
					bValidTarget = false;
				}
				else
				{
					LeapTarget = floorTrace.ImpactPoint;
				}
			}
		}
		//case if the player aims at the ground close to character
		else if(hitResult.Distance < MinTargetDistance)
		{
			LeapTarget = FVector(actorLoc.X, actorLoc.Y, hitResult.ImpactPoint.Z) + camViewRotator.Vector() * MinTargetDistance;
		}
	}
	else
	{
		FHitResult floorTrace;
		world->LineTraceSingleByProfile(floorTrace, end, end + FVector::DownVector * 10000.f, "AbilityTrace");

		if(floorTrace.bBlockingHit)
		{
			//This check is needed when player aims straight into the air
			float distanceToActor = (floorTrace.ImpactPoint - actorLoc).Size();
			if(distanceToActor < MinTargetDistance)
			{
				LeapTarget = FVector(actorLoc.X, actorLoc.Y, hitResult.ImpactPoint.Z) + camViewRotator.Vector() * MinTargetDistance;
			}
			else
			{
				LeapTarget = floorTrace.ImpactPoint;
			}
		}
	}

	if(bValidTarget)
	{
		DrawDebugCylinder(world, LeapTarget, LeapTarget + FVector::UpVector * 100.f, TargetIndicatorRadius, 12, FColor::Red, false, 0.01f, 0, 5);
	}
}

void UEarthAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrState == EAbilityState::ABILITY_FIRE)
	{
		Fire();
	}
	else if (CurrState == EAbilityState::ABILITY_TARGETING)
	{
		Targeting();
	}
}

void UEarthAbility::EndLeap()
{
	auto cooldownComponent = GetOwner()->FindComponentByClass<UCooldownComponent>();
	cooldownComponent->StartCooldown("Ult");
	SetAbilityState(EAbilityState::ABILITY_COOLDOWN);
	bValidTarget = false;
	LeapTarget = FVector::ZeroVector;
}

void UEarthAbility::DoLeap()
{
	float elapsedLeapTime = GetWorld()->GetTimerManager().GetTimerElapsed(LeapTimerHandle);

	if(elapsedLeapTime < 0.f)
	{
		UE_LOG(LogActor, Error, TEXT("EarthAbility: Leap Timer is not set. Leap no work"));
		return;
	}

	elapsedLeapTime /= LeapDuration;
	FVector leapMiddle = LeapStart + (LeapTarget - LeapStart) / 2.f + FVector::UpVector * LeapHeight;

	FVector startToMiddle = FMath::Lerp<FVector>(LeapStart, leapMiddle, elapsedLeapTime);
	FVector middleToEnd = FMath::Lerp<FVector>(leapMiddle, LeapTarget, elapsedLeapTime);
	FVector leapLocation = FMath::Lerp<FVector>(startToMiddle, middleToEnd, elapsedLeapTime);

	GetOwner()->SetActorLocation(leapLocation, true);
}

void UEarthAbility::BeginPlay()
{
	Super::BeginPlay();
	
	character = Cast<ACyberbowlCharacter>(GetOwner());

}

void UEarthAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrState == EAbilityState::ABILITY_FIRE)
	{
		SpawnPillar();
	}

	if (!characterController)
	{
		characterController = Cast<AThirdPersonPlayerController>(character->GetController());
	}
}

void UEarthAbility::SpawnPillar()
{
	FVector spawnLocation = FVector(character->GetActorLocation().X + 150, character->GetActorLocation().Y, character->GetActorLocation().Z);
	float yawRotation = character->GetCameraBoom()->GetTargetRotation().Yaw - 180;
	pillar = GetWorld()->SpawnActor<AEarthpillar>(earthClass, spawnLocation, FRotator(20, yawRotation, 0));
	pillar->SetLifeSpan(4.f);
	pillar->SetCurrPlayerTeam(characterController->currPlayerTeam);
	

	auto cooldownComponent = character->FindComponentByClass<UCooldownComponent>();
	cooldownComponent->StartCooldown("Ult");
	SetAbilityState(EAbilityState::ABILITY_COOLDOWN);
}
