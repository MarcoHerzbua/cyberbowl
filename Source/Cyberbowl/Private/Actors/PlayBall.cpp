// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlayBall.h"

#include "Engine/Engine.h"

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

	FVector ballVelocity = GetVelocity();
	BallStaticMesh->SetAllPhysicsLinearVelocity( ballVelocity.GetClampedToMaxSize(MaxSpeed)) ;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *GetVelocity().ToString()));
}

void APlayBall::PushBall(float force, FVector direction)
{
	OnBallBooped.Broadcast();
	
	//BallStaticMesh->ComponentVelocity = FVector(0);
	//Is this only moving the mesh? -> Marco: the mesh a scene component and the root of the actor, so everything in this actor is moved
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

void APlayBall::Freeze_Implementation(AActor* instigtr)
{
	BallStaticMesh->SetSimulatePhysics(false);
}

void APlayBall::UnFreeze_Implementation()
{
	BallStaticMesh->SetSimulatePhysics(true);
}

