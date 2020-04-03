// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Earthpillar.h"
#include "Character/CyberbowlCharacter.h"
#include "PlayerController/ThirdPersonPlayerController.h"

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
	triggerMesh->OnComponentEndOverlap.AddDynamic(this, &AEarthpillar::EndOverlap);
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

}

void AEarthpillar::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACyberbowlCharacter* overlappingActor = Cast<ACyberbowlCharacter>(OtherActor);
	if(overlappingActor)
	{
		
		AThirdPersonPlayerController* controller = Cast<AThirdPersonPlayerController>(overlappingActor->GetController());
		if (controller->currPlayerTeam == currPlayerTeam)
		{
			bIsRising = true;
			UE_LOG(LogTemp, Warning, TEXT("I'am on the pillar and a Character :D"));
		}
	}
	else
	{
		bIsRising = true;
		UE_LOG(LogTemp, Warning, TEXT("I'am on the pillar and a Ball :D"));
	}
}

void AEarthpillar::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACyberbowlCharacter* overlappingActor = Cast<ACyberbowlCharacter>(OtherActor);
	if (overlappingActor)
	{
		//bIsLowering = true;
		//UE_LOG(LogTemp, Warning, TEXT("I'am off the pillar and a Character D:"));
	}
	else
	{
		//bIsLowering = true;
		//UE_LOG(LogTemp, Warning, TEXT("I'am off the pillar and a Ball D:"));
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

void AEarthpillar::SetCurrPlayerTeam(int playerTeam)
{
	currPlayerTeam = playerTeam;
}

void AEarthpillar::SetMaxLoweringPos(float pos)
{
	maxLowering = pos;
}
