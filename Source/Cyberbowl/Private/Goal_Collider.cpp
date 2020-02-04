// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal_Collider.h"

// Sets default values
AGoal_Collider::AGoal_Collider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoal_Collider::BeginPlay()
{
	Super::BeginPlay();

	if (ball)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball is set!"));
	}
	
}

// Called every frame
void AGoal_Collider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnActorBeginOverlap;
	if (IsOverlappingActor(ball))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball overlapping!"));
	}

}

