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
	character = Cast<ACyberbowlCharacter>(GetOwner());
	camera = character->GetFollowCamera();
	fireWallPosition = FVector(0.f);
	bValidTarget = false;
	bValidTargetBoxSize = false;
	boxScale = FVector(0.f);
	fireWallLifeTime = 8.f;
	
}


void UFireAbility::Fire()
{
	if(!bValidTarget)
	{
		SetAbilityState(EAbilityState::ABILITY_DEFAULT);
		return;
	}

	AFirewall* firewall = GetWorld()->SpawnActor<AFirewall>(fireClass);
	
	FVector boxPosition = fireWallPosition +FVector(0.f, 0.f, firewall->GetBoxExtent().Z);
	firewall->SetActorLocation(boxPosition);
	FRotator cam = camera->GetRelativeRotation();

	auto camRotation = character->GetCameraBoom()->GetTargetRotation();
	FRotator rotation = FRotator(0.f, camRotation.Yaw + 90, 0.f);
	firewall->SetActorRotation(rotation);

	firewall->SetLifeSpan(fireWallLifeTime);

	auto cooldownComponent = character->FindComponentByClass<UCooldownComponent>();
	cooldownComponent->StartCooldown("Ult");
	SetAbilityState(EAbilityState::ABILITY_COOLDOWN);
	bValidTarget = false;
	bValidTargetBoxSize = false;
	
}

void UFireAbility::Targeting()
{
	//GetWorld()->Ca
	UWorld* world = GetWorld();
	APlayerCameraManager* camManager = world->GetFirstPlayerController()->PlayerCameraManager;
	FVector cameraPos = camManager->GetCameraLocation();
	FVector cameraView = camManager->GetActorForwardVector();
	FVector end = character->GetActorLocation() + cameraView * targetingLength;

	if (!bValidTargetBoxSize)
	{
		AFirewall* dummyActor = GetWorld()->SpawnActor<AFirewall>(fireClass);
		boxScale = dummyActor->GetBoxExtent();
		GetWorld()->DestroyActor(dummyActor);
	}

	bValidTargetBoxSize = true;
	
	FRotator cam = camera->GetRelativeRotation();
	auto camRotation = character->GetCameraBoom()->GetTargetRotation();
	FRotator rotation = FRotator(0.f, camRotation.Yaw + 90, 0.f);
	
	FHitResult hitResult;
	world->LineTraceSingleByProfile(hitResult, cameraPos, end, "AbilityTrace");
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
				DrawDebugBox(world, floorTrace.ImpactPoint, FVector(boxScale.X, boxScale.Y, 10), rotation.Quaternion(), FColor::Blue, false, 0.1f, 0, 5.f);
				fireWallPosition = floorTrace.ImpactPoint;
			}

		}
		else
		{
			DrawDebugBox(world, hitResult.ImpactPoint, FVector(boxScale.X, boxScale.Y, 10), rotation.Quaternion(), FColor::Blue, false, 0.1f, 0, 5.f);
		}
	}
	else
	{
		FHitResult floorTrace;
		world->LineTraceSingleByProfile(floorTrace, end, end + FVector::DownVector * 10000.f, "AbilityTrace");

		if (floorTrace.bBlockingHit)
		{
			float distanceToActor = (floorTrace.ImpactPoint - character->GetActorLocation()).Size();
			fireWallPosition = floorTrace.ImpactPoint;
			bValidTarget = true;
			DrawDebugBox(world, floorTrace.ImpactPoint, FVector(boxScale.X, boxScale.Y, 10), rotation.Quaternion(), FColor::Blue, false, 0.1f, 0, 5.f);
		}
	}
	
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

