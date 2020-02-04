// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BoopComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Actors/PlayBall.h"

// Sets default values for this component's properties
UBoopComponent::UBoopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBoopComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	MovementComponent = Owner->FindComponentByClass<UCharacterMovementComponent>();
}

void UBoopComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

void UBoopComponent::StartBoop()
{
	auto cameraMngr = UGameplayStatics::GetPlayerCameraManager(Owner, UGameplayStatics::GetPlayerControllerID(PlayerController));

	FVector cameraForwardVec = cameraMngr->GetActorForwardVector();
	FRotator cameraRotation = cameraMngr->K2_GetActorRotation();

	TArray<FHitResult> hits;
	FVector start = Owner->GetActorLocation();
	FVector end = start + cameraForwardVec * Range;
	
	auto world = Owner->GetWorld();
	if(world->SweepMultiByProfile(hits, start, end, cameraRotation.Quaternion(), "PlayBall", FCollisionShape::MakeBox(FVector(100.f, BoxWidth, BoxHeight))))
	{
	}

	DrawDebugBox(world, start + (cameraForwardVec * Range) / 2, FVector(Range / 2, BoxWidth, BoxHeight), cameraRotation.Quaternion(), FColor::Red, false, 3.f, 0, 3.f);

	for(auto hit : hits)
	{
		auto ball = Cast<APlayBall>(hit.Actor);
		if(ball)
		{
			FVector direction = cameraForwardVec;
			if(MovementComponent->MovementMode == EMovementMode::MOVE_Walking)
			{
				direction = direction.RotateAngleAxis(UpwardsAngle, FVector(1, 0, 0));
			}
			ball->PushBall(Force, direction);
		}
	}


}


// Called every frame
void UBoopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//these assignments do not work in BeginPlay(), because playercontroller is assigned afterwards at some point
	if(!PlayerController)
	{
		auto pawn = Cast<APawn>(Owner);
		if (!pawn)
		{
			return;
		}

		auto pc = Cast<APlayerController>(pawn->GetController());

		auto id = UGameplayStatics::GetPlayerControllerID(pc);
		PlayerController = pc;
	}

	//does not work in BeginPlay(), same as above
	if (!InputComponent)
	{
		InputComponent = Owner->InputComponent;
		if (InputComponent)
		{
			InputComponent->BindAction("Boop", IE_Pressed, this, &UBoopComponent::StartBoop);
		}
	}
}

