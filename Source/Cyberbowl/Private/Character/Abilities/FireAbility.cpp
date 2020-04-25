// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Abilities/FireAbility.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Character/Abilities/AbilityUtils.h"
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

	AFirewall* firewall = GetWorld()->SpawnActor<AFirewall>(FireWallClass);
	
	FVector boxPosition = fireWallPosition + FVector(0.f, 0.f, firewall->GetBoxExtent().Z);
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
	auto actorRot = character->GetCameraBoom()->GetTargetRotation();
	//Set Pitch to zero cause we are only interested in the Yaw (Roll is always 0)
	actorRot.Pitch = 0.f;
	FRotator indicatorRotation = FRotator(0.f, actorRot.Yaw + 90, 0.f);
	FVector end = actorLoc + actorRot.Vector() * TargetDistance;

	if (!bValidTargetBoxSize)
	{
		AFirewall* dummyActor = GetWorld()->SpawnActor<AFirewall>(FireWallClass);
		boxScale = dummyActor->GetBoxExtent();
		GetWorld()->DestroyActor(dummyActor);
		bValidTargetBoxSize = true;
	}

	bValidTarget = UAbilityUtils::FindTargetPoint(world, fireWallPosition, actorLoc, end, 100.f);

	DrawDebugBox(world, fireWallPosition, FVector(boxScale.X, boxScale.Y, 10), indicatorRotation.Quaternion(), FColor::Blue, false, 0.01f, 0, 10.f);
	
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

