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
	validTarget = false;
	fireWallLifeTime = 8.f;
	
}


void UFireAbility::Fire()
{
	if(!validTarget)
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
	validTarget = false;
	
}

void UFireAbility::Targeting()
{
	//GetWorld()->Ca
	UWorld* world = GetWorld();
	APlayerCameraManager* camManager = world->GetFirstPlayerController()->PlayerCameraManager;
	FVector cameraPos = camManager->GetCameraLocation();
	FVector cameraView = camManager->GetActorForwardVector();
	FVector end = character->GetActorLocation() + cameraView * targetingLength;

	AFirewall* dummyActor = GetWorld()->SpawnActor<AFirewall>(fireClass);
	FVector boxScale = dummyActor->GetBoxExtent();
	GetWorld()->DestroyActor(dummyActor);
	FRotator cam = camera->GetRelativeRotation();
	auto camRotation = character->GetCameraBoom()->GetTargetRotation();
	FRotator rotation = FRotator(0.f, camRotation.Yaw + 90, 0.f);
	
	FHitResult hitResult;
	world->LineTraceSingleByProfile(hitResult, cameraPos, end, "FireAbilityTrace");
	DrawDebugBox(world, end, FVector(5.f), FColor::Red, false, 5, 0, 3);


	if (hitResult.bBlockingHit && hitResult.Normal == FVector(0.f, 0.f, 1.f))
	{
		DrawDebugBox(world, hitResult.ImpactPoint, FVector(boxScale.X, boxScale.Y, 10), rotation.Quaternion(), FColor::Blue, false, 0.1f, 0, 5.f);
		fireWallPosition = hitResult.ImpactPoint;
		validTarget = true;
	}
	else
	{
		validTarget = false;
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

