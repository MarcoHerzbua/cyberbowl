// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Abilities/FireAbility.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Abilities/CooldownComponent.h"
#include "DrawDebugHelpers.h"

void UFireAbility::BeginPlay()
{
	Super::BeginPlay();
	fireWallPosition = FVector(0.f);
	bValidTarget = false;
	bValidTargetBoxSize = false;
	boxScale = FVector(0.f);
	fireWallLifeTime = 8.f;
	
}


void UFireAbility::Fire()
{
	ACyberbowlCharacter* character = Cast<ACyberbowlCharacter>(GetOwner());
	if(!bValidTarget)
	{
		SetAbilityState(EAbilityState::ABILITY_DEFAULT);
		return;
	}

	AFirewall* firewall = GetWorld()->SpawnActor<AFirewall>(fireClass);
	
	FVector boxPosition = fireWallPosition +FVector(0.f, 0.f, firewall->GetBoxExtent().Z);
	firewall->SetActorLocation(boxPosition);

	auto rotation = character->GetCameraBoom()->GetTargetRotation();
	FRotator indicatorRotation = FRotator(0.f, rotation.Yaw + 90, 0.f);
	firewall->SetActorRotation(indicatorRotation);

	firewall->SetLifeSpan(fireWallLifeTime);

	auto cooldownComponent = character->FindComponentByClass<UCooldownComponent>();
	cooldownComponent->StartCooldown("Ult");
	SetAbilityState(EAbilityState::ABILITY_COOLDOWN);
	bValidTarget = false;
	bValidTargetBoxSize = false;
	
}

void UFireAbility::Targeting()
{
	UWorld* world = GetWorld();

	ACyberbowlCharacter* character = Cast<ACyberbowlCharacter>(GetOwner());
	FVector actorLoc = character->GetActorLocation();
	auto rotation = character->GetCameraBoom()->GetTargetRotation();
	FRotator indicatorRotation = FRotator(0.f, rotation.Yaw + 90, 0.f);
	FVector end = actorLoc + rotation.Vector() * targetingLength;


	if (!bValidTargetBoxSize)
	{
		AFirewall* dummyActor = GetWorld()->SpawnActor<AFirewall>(fireClass);
		boxScale = dummyActor->GetBoxExtent();
		GetWorld()->DestroyActor(dummyActor);
	}

	bValidTargetBoxSize = true;
	

	
	FHitResult hitResult;
	world->LineTraceSingleByProfile(hitResult, actorLoc, end, "AbilityTrace");
	DrawDebugBox(world, end, FVector(5.f), FColor::Red, false, 5, 0, 3);


	if (hitResult.bBlockingHit)
	{
		fireWallPosition = hitResult.ImpactPoint;
		bValidTarget = true;

		if (hitResult.GetComponent()->GetCollisionProfileName() == "StadiumWall")
		{

			//get a point a certain distant away from the wall
			FVector hitPointWithOffset = hitResult.ImpactPoint + hitResult.ImpactNormal * targetIndicatorRadius;

			FHitResult floorTrace;
			world->LineTraceSingleByProfile(floorTrace, hitPointWithOffset, hitPointWithOffset + FVector::DownVector * 10000.f, "AbilityTrace");

			if (floorTrace.bBlockingHit)
			{
				bValidTarget = true;

				fireWallPosition = floorTrace.ImpactPoint;
			}

		}
	}
	else
	{
		FHitResult floorTrace;
		world->LineTraceSingleByProfile(floorTrace, end, end + FVector::DownVector * 10000.f, "AbilityTrace");

		if (floorTrace.bBlockingHit)
		{
			float distanceToActor = (floorTrace.ImpactPoint - actorLoc).Size();
			fireWallPosition = floorTrace.ImpactPoint;
			bValidTarget = true;
		}
	}

	DrawDebugBox(world, fireWallPosition, FVector(boxScale.X, boxScale.Y, 10), indicatorRotation.Quaternion(), FColor::Blue, false, 0.1f, 0, 5.f);
	
}

void UFireAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

