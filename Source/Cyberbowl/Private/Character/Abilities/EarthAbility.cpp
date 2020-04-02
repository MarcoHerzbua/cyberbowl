// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/EarthAbility.h"
#include "Character/CyberbowlCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Abilities/CooldownComponent.h"
#include "PlayerController/ThirdPersonPlayerController.h"

void UEarthAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Earth)Pillar-Thingy: ")));
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
