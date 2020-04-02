// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/EarthAbility.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UEarthAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Earth)Pillar-Thingy: ")));
	SetAbilityState(EAbilityState::ABILITY_COOLDOWN);
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

	FVector indicatorSpawnVec;
	FRotator camViewRotator = ownerAsPawn->GetControlRotation();
	//Set Pitch to zero cause we are only interested in the Yaw (Roll is always 0)
	camViewRotator.Pitch = 0.f;
	
	if (hitResult.bBlockingHit)
	{
		MaxTargetDistance = 3000.f;
		indicatorSpawnVec = hitResult.ImpactPoint;
		if(hitResult.GetComponent()->GetCollisionProfileName() == "StadiumWall")
		{
			FVector hitPointWithOffset = hitResult.ImpactPoint + hitResult.ImpactNormal * 250.f;

			FHitResult floorTrace;
			world->LineTraceSingleByProfile(floorTrace, hitPointWithOffset, hitPointWithOffset + FVector::DownVector * 10000.f, "AbilityTrace");

			if (floorTrace.bBlockingHit)
			{
				float distanceToActor = (floorTrace.ImpactPoint - actorLoc).Size();
				if (distanceToActor < MinTargetDistance)
				{
					//exception: this should block player from firing
				}
				else
				{
					indicatorSpawnVec = floorTrace.ImpactPoint;
				}

			}

		}
		else if(hitResult.Distance < MinTargetDistance)
		{
			indicatorSpawnVec = FVector(actorLoc.X, actorLoc.Y, hitResult.ImpactPoint.Z) + camViewRotator.Vector() * MinTargetDistance;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("%f"), hitResult.Distance));
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
				indicatorSpawnVec = FVector(actorLoc.X, actorLoc.Y, hitResult.ImpactPoint.Z) + camViewRotator.Vector() * MinTargetDistance;
			}
			else
			{
				indicatorSpawnVec = floorTrace.ImpactPoint;
			}
		}
	}
	
	DrawDebugCylinder(world, indicatorSpawnVec, indicatorSpawnVec + FVector::UpVector * 100.f, 250.f, 12, FColor::Red, false, 0.01f, 0, 5);
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
