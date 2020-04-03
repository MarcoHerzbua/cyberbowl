// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Earthpillar.h"

#include "Actors/PlayBall.h"
#include "Character/CyberbowlCharacter.h"
#include "PlayerController/ThirdPersonPlayerController.h"
#include "TimerManager.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Engine/Engine.h"
// Sets default values
AEarthpillar::AEarthpillar()
{
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(FName("EarthPillarRoot"));
	RootComponent = root;
	cylinder = CreateDefaultSubobject<UStaticMeshComponent>(FName("CylinderMesh"));
	cylinder->SetupAttachment(root);
	cylinder->SetCollisionProfileName("BlockAllDynamic");
	triggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("TriggerMesh"));
	triggerMesh->SetupAttachment(root);
	triggerMesh->SetCollisionProfileName("OverlapAllDynamic");
	currPlayerTeam = -1;
}


// Called when the game starts or when spawned
void AEarthpillar::BeginPlay()
{
	Super::BeginPlay();
	triggerMesh->OnComponentBeginOverlap.AddDynamic(this, &AEarthpillar::BeginOverlap);
}

// Called every frame
void AEarthpillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float val = GetActorLocation().Z;

	if ((GetActorLocation().Z) >= maxRise)
	{
		bIsRising = false;
		triggerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (bIsRising && GetActorLocation().Z <= maxRise)
	{
		Rising(DeltaTime);
		
	}

	else if (val > maxLowering)
	{
		Lowering(DeltaTime);
	}

	else
	{
		triggerMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	TickLaunch();
}

void AEarthpillar::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACyberbowlCharacter* overlappingActor = Cast<ACyberbowlCharacter>(OtherActor);
	if(overlappingActor)
	{
		
		AThirdPersonPlayerController* controller = Cast<AThirdPersonPlayerController>(overlappingActor->GetController());
		if (controller->currPlayerTeam == currPlayerTeam)
		{
			LaunchActor(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("I'am on the pillar and a Character :D"));
		}
	}
	else
	{
		bIsRising = true;
		//LaunchActor(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("I'am on the pillar and a Ball :D"));
	}
}

void AEarthpillar::Rising(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + GetActorUpVector() * FVector(riseTime * DeltaTime));
}

void AEarthpillar::Lowering(float DeltaTime)
{
	SetActorLocation(GetActorLocation() - GetActorUpVector() * FVector(riseTime * DeltaTime));
}

void AEarthpillar::TickLaunch()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(LaunchTimerHandle) || !LaunchedActor)
	{
		return;
	}

	float elapsedLeapTime = GetWorld()->GetTimerManager().GetTimerElapsed(LaunchTimerHandle);
	elapsedLeapTime /= LaunchDuration;
	
	FVector leapMiddle = LaunchStart + (LaunchTarget - LaunchStart) / 2.f + FVector::UpVector * LaunchHeight;

	FVector startToMiddle = FMath::Lerp<FVector>(LaunchStart, leapMiddle, elapsedLeapTime);
	FVector middleToEnd = FMath::Lerp<FVector>(leapMiddle, LaunchTarget, elapsedLeapTime);
	FVector leapLocation = FMath::Lerp<FVector>(startToMiddle, middleToEnd, elapsedLeapTime);

	LaunchedActor->SetActorLocation(leapLocation, true);
}

void AEarthpillar::EndLaunch()
{
	LaunchedActor = nullptr;
}

void AEarthpillar::SetCurrPlayerTeam(int playerTeam)
{
	currPlayerTeam = playerTeam;
}

void AEarthpillar::SetMaxLoweringPos(float pos)
{
	maxLowering = pos;
}

void AEarthpillar::InitializePillar(FVector launchTarget, float launchDuration, float launchHeight)
{
	LaunchTarget = launchTarget;
	LaunchDuration = launchDuration;
	LaunchHeight = launchHeight;
}

void AEarthpillar::LaunchActor(AActor* actor)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(LaunchTimerHandle))
	{
		if(GetLifeSpan() < LaunchDuration)
		{
			SetLifeSpan(LaunchDuration);
		}
		LaunchedActor = actor;
		LaunchStart = actor->GetActorLocation();
		bIsRising = true;
		GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, this, &AEarthpillar::EndLaunch, LaunchDuration/* - LaunchDuration / 10.f*/);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EarthPillar: Another Actor is currently launching"));
	}
}
