// Fill out your copyright notice in the Description page of Project Settings.

#include "Goal_Collider.h"
#include <string>

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
	BoxComponent = FindComponentByClass<UBoxComponent>();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGoal_Collider::OnBeginOverlap);
}

// Called every frame
void AGoal_Collider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Ball)
	{
		APoints* CurrMode = Cast<APoints>(GetWorld()->GetAuthGameMode());
		if (CurrMode && CurrMode->Ball)
		{
			Ball = CurrMode->Ball;
			UE_LOG(LogTemp, Warning, TEXT("Ball initialized!"));
		}

	}
	
}

void AGoal_Collider::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin overlap"));
	if (OtherActor == Ball)
	{
		APoints* CurrMode = Cast<APoints>(GetWorld()->GetAuthGameMode());
		CurrMode->Add_Points(this);

	}
}
