// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlayBall.h"

// Sets default values
APlayBall::APlayBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayBall::BeginPlay()
{
	Super::BeginPlay();
	
	BallStaticMesh = FindComponentByClass<UStaticMeshComponent>();
	if (!BallStaticMesh)
	{
		return;
	}
	BallStaticMesh->SetWorldScale3D(FVector(ScaleModifier));
	StartPosition = GetActorLocation();
}

// Called every frame
void APlayBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayBall::PushBall(float force, FVector direction)
{
	//BallStaticMesh->ComponentVelocity = FVector(0);
	//Is this only moving the mesh?
	BallStaticMesh->AddImpulse(direction * force, NAME_None, true);
}

void APlayBall::PlayBall()
{
	BallStaticMesh->SetEnableGravity(true);
	BallStaticMesh->SetSimulatePhysics(true);
}

void APlayBall::StopBall()
{
	BallStaticMesh->SetEnableGravity(false);
	BallStaticMesh->SetSimulatePhysics(false);
}

void APlayBall::ResetBallPosition()
{
	BallStaticMesh->SetWorldLocation(StartPosition);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *StartPosition.ToString());
}

