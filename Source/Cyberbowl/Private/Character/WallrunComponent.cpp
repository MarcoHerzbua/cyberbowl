// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WallrunComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Character/CBCharacterMovementComponent.h"



// Sets default values for this component's properties
UWallrunComponent::UWallrunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWallrunComponent::BeginPlay()
{
	Super::BeginPlay();

	auto sphereCmps = GetOwner()->GetComponentsByTag(USphereComponent::StaticClass(), "WallrunCollider");

	if(sphereCmps.Num() >= 1)
	{
		WallrunCollider = Cast<USphereComponent>(sphereCmps[0]);
	}

	if(!WallrunCollider)
	{
		UE_LOG(LogInit, Error, TEXT("No WallrunCollider set in Character Blueprint"));
	}
	else
	{
		WallrunCollider->OnComponentBeginOverlap.AddDynamic(this, &UWallrunComponent::CheckForWallrun);
		WallrunCollider->OnComponentEndOverlap.AddDynamic(this, &UWallrunComponent::EndWallrun);
		//WallrunCollider->OnComponentHit.AddDynamic(this, &UWallrunComponent::CheckForWallrun);
	}

	MovementComponent = GetOwner()->FindComponentByClass<UCBCharacterMovementComponent>();
}

void UWallrunComponent::CheckForWallrun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto test = JumpInputPressedDuration;
	//TODO: Should not be handled by tags, but Stadium should have its own Blueprint and walls should have a collision preset (or something similiar) 
	if(!OtherComp->ComponentTags.Contains("Stadium"))
	{
  		return;
	}

	if(MovementComponent->MovementMode == EMovementMode::MOVE_Falling)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("WallrunCmp valid Wallrun")));
		
		//push character against the wall 
		MovementComponent->AddImpulse(SweepResult.Normal * 1000.f);
		
		MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_Wallrun);
	}
}

void UWallrunComponent::EndWallrun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_Running);
}


// Called every frame
void UWallrunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Movementmode: %i"), int(MovementComponent->MovementMode)));
	//TODO: not supposed to be here; some of the logic in this class should be outsourced to custom playercontroller
	if (!PlayerController)
	{
		auto pawn = Cast<APawn>(GetOwner());
		if (!pawn)
		{
			return;
		}

		auto pc = Cast<APlayerController>(pawn->GetController());

		PlayerController = pc;
		JumpKeyMapping = PlayerController->PlayerInput->GetKeysForAction("Jump");
	}

	if(PlayerController)
	{
		JumpInputPressedDuration = 0.f;
		for(auto keyMapping : JumpKeyMapping)
		{
			float oldDuration = JumpInputPressedDuration;
			JumpInputPressedDuration = PlayerController->GetInputKeyTimeDown(keyMapping.Key);

			if(oldDuration > JumpInputPressedDuration)
			{
				JumpInputPressedDuration = oldDuration;
			}
		}
	}
}

