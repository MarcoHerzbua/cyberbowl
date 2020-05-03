// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WallrunComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Character/MovementStates/WallrunState.h"
#include "Character/BoopComponent.h"


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

	auto boopComponent = GetOwner()->FindComponentByClass<UBoopComponent>();
	if(!WallrunCollider || !boopComponent)
	{
		UE_LOG(LogInit, Error, TEXT("WallrunComponent: Vital Components not set in Character Blueprint! (WallrunCollider, boopcomponent)"));
	}
	else
	{
		WallrunCollider->OnComponentBeginOverlap.AddDynamic(this, &UWallrunComponent::CheckForWallrun);
		WallrunCollider->OnComponentEndOverlap.AddDynamic(this, &UWallrunComponent::EndWallrun);
		boopComponent->OnBoopStarted.AddDynamic(this, &UWallrunComponent::ForceEndWallrun);
	}

	MovementComponent = GetOwner()->FindComponentByClass<UCBCharacterMovementComponent>();
}

void UWallrunComponent::CheckForWallrun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (MovementComponent->GetCBMovementMode() != ECBMovementMode::CBMOVE_Running)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("WallrunCmp valid Wallrun")));
		//CountWallTouches++;

		//push character against the wall 
		MovementComponent->AddImpulse(SweepResult.Normal * 1000.f);

		MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_Wallrun);
		OnWallrunStart.Broadcast();
	}
}

void UWallrunComponent::EndWallrun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//This check is necessary to avoid bugs when character just walks up to a wall 
	if(MovementComponent->GetCBMovementMode() == ECBMovementMode::CBMOVE_Wallrun)
	{
		//CountWallTouches--;
		//if(CountWallTouche == 0)

		auto movementState = Cast<UWallrunState>(MovementComponent->GetCBMovementState());

		if(!movementState->IsLaunching())
		{
			MovementComponent->SetCBMovementMode(ECBMovementMode::CBMOVE_Jump);
		}
	}

	//TODO: Wallrun Eventdispatchers need to be rearranged/cleared up because its no bueno
	OnWallrunEnd.Broadcast();
}

void UWallrunComponent::ForceEndWallrun()
{
	EndWallrun(nullptr, nullptr, nullptr, 0);
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

		if(!pc)
		{
			return;
		}
		PlayerController = pc;
		JumpKeyMapping = PlayerController->PlayerInput->GetKeysForAction("Jump");
	}

	//if(MovementComponent->GetCBMovementMode() != ECBMovementMode::CBMOVE_Wallrun)
	//{
	//	CountWallTouches = 0;
	//}

	//if(PlayerController)
	//{
	//	JumpInputPressedDuration = 0.f;
	//	for(auto keyMapping : JumpKeyMapping)
	//	{
	//		float oldDuration = JumpInputPressedDuration;
	//		JumpInputPressedDuration = PlayerController->GetInputKeyTimeDown(keyMapping.Key);

	//		if(oldDuration > JumpInputPressedDuration)
	//		{
	//			JumpInputPressedDuration = oldDuration;
	//		}
	//	}
	//}
}

