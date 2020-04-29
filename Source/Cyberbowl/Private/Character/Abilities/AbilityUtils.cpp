#pragma once

#include "Character/Abilities/AbilityUtils.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

bool UAbilityUtils::FindTargetPoint(UWorld* worldContext, FVector& targetPoint, FVector traceStart, FVector traceEnd, float wallOffset)
{
	FHitResult hitResult;
	worldContext->LineTraceSingleByProfile(hitResult, traceStart, traceEnd, "AbilityTrace");

	bool foundValidTarget = true;
	FVector floorTraceStart;
	if (hitResult.bBlockingHit)
	{
		if (hitResult.GetComponent()->GetCollisionProfileName() == "StadiumWall")
		{
			//get a point a certain distant away from the wall
			floorTraceStart = hitResult.ImpactPoint + hitResult.ImpactNormal * wallOffset;
		}
		else
		{
			UE_LOG(LogActor, Error, TEXT("AbilityTargeting: Error when tracing for target location."));
			return false;
			//return;
		}
	}
	else
	{
		floorTraceStart = traceEnd;
	}

	FHitResult floorTrace;
	worldContext->LineTraceSingleByProfile(floorTrace, floorTraceStart, floorTraceStart + FVector::DownVector * 10000.f, "AbilityTrace");

	if (floorTrace.bBlockingHit)
	{
		targetPoint = floorTrace.ImpactPoint;
	}
	else
	{
		UE_LOG(LogActor, Error, TEXT("AbilityTargeting: Error when tracing for target location."));
		return false;
	}

	return true;
}
