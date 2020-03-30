// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/IceAbility.h"

#include "Actors/IFreezeable.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

#include "Kismet/KismetSystemLibrary.h"

void UIceAbility::Fire()
{
	float coneAngleInRadians = FMath::DegreesToRadians(ConeAngle);
	float coneRadius = ConeLength * FMath::Tan(ConeAngle) / 2.f;
	APawn* ownerAsPawn = Cast<APawn>(GetOwner());

	FCollisionShape shape = FCollisionShape::MakeSphere(coneRadius);
	FVector start = GetOwner()->GetActorLocation();
	FVector direction = ownerAsPawn->GetControlRotation().Vector();
	FVector end = start + direction * ConeLength;
	TArray<FHitResult> hitResults;

	//DEBUG: Draw Cone Shape
	DrawDebugCone(GetOwner()->GetWorld(), start, direction, ConeLength, coneAngleInRadians, coneAngleInRadians, 12, FColor::Blue, false, FreezeDuration, 0, 4.f);
	//DrawDebugSphere(GetOwner()->GetWorld(), GetOwner()->GetActorLocation(), coneRadius, 12, FColor::Emerald, false, 4, 0, 2.f);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("%f"), coneRadius));

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());
	GetOwner()->GetWorld()->SweepMultiByProfile(hitResults, start + direction * coneRadius, end - direction * coneRadius, FQuat(), "OverlapDynamics", shape, queryParams);

	TArray<AActor*> hitActors;
	for(auto hit : hitResults)
	{
		if(!hitActors.Contains(hit.GetActor()) && IsWithinCone(hit.ImpactPoint, direction))
		{
			hitActors.Add(hit.GetActor());
			DrawDebugPoint(GetOwner()->GetWorld(), hit.ImpactPoint, 10.f, FColor::Black, false, 5.f);
		}
	}

	for(auto actor : hitActors)
	{
		if(actor->GetClass()->ImplementsInterface(UFreezeable::StaticClass()))
		{
			IFreezeable::Execute_Freeze(actor, GetOwner());
			FrozenActors.Add(actor);
		}
	}

	if(FrozenActors.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(FreezeTimerHandle, this, &UIceAbility::UnfreezeActors, FreezeDuration);
	}
}

void UIceAbility::UnfreezeActors()
{
	for(auto actor : FrozenActors)
	{
		if (actor->GetClass()->ImplementsInterface(UFreezeable::StaticClass()))
		{
			IFreezeable::Execute_UnFreeze(actor);
		}
	}
	FrozenActors.Empty();
}

bool UIceAbility::IsWithinCone(FVector hitPoint, FVector coneDirectionNormal)
{
	FVector actorLoc = GetOwner()->GetActorLocation();
	FVector hitDirection = hitPoint - actorLoc;

	float angleRadians = FMath::Acos(FVector::DotProduct(coneDirectionNormal, hitDirection.GetSafeNormal()));
	float angleDegrees = FMath::RadiansToDegrees(angleRadians);

	FVector projectedHitPoint = hitDirection.ProjectOnToNormal(coneDirectionNormal);
	float length = projectedHitPoint.Size();
	
	return angleDegrees <= ConeAngle && length <= ConeLength;
}
