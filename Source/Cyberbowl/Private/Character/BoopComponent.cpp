// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BoopComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Actors/PlayBall.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"



// Sets default values for this component's properties
UBoopComponent::UBoopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//BoopHitbox = CreateDefaultSubobject<UBoxComponent>(FName("BoopHitbox"));
}


// Called when the game starts
void UBoopComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	MovementComponent = Owner->FindComponentByClass<UCBCharacterMovementComponent>();

	//BoopHitbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//BoopHitbox->SetWorldLocationAndRotation(Owner->GetActorLocation() + FVector(Range / 2.f,0,0), Owner->GetActorRotation());
	////BoopHitbox->SetupAttachment(this->Owner->GetRootComponent());
	//BoopHitbox->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//BoopHitbox->SetBoxExtent(FVector(Range, BoxWidth, BoxHeight));

	auto boxComps = GetOwner()->GetComponentsByTag(UBoxComponent::StaticClass(), "BoopHitbox");

	if (boxComps.Num() >= 1)
	{
		BoopHitbox = Cast<UBoxComponent>(boxComps[0]);
	}

	if (!BoopHitbox)
	{
		UE_LOG(LogInit, Error, TEXT("No BoopHitbox set in Character Blueprint"));
	}
	else
	{
		BoopHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoopHitbox->OnComponentBeginOverlap.AddDynamic(this, &UBoopComponent::PushBall);
	}
}

void UBoopComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(BoopDurationHandle);
}

void UBoopComponent::StartBoop()
{
	OnBoopStarted.Broadcast();

	if (!bBoopActive)
	{
		Owner->GetWorld()->GetTimerManager().SetTimer(BoopDurationHandle, this, &UBoopComponent::DeactivateBoopHitbox, Duration);

		BoopHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bBoopActive = true;
	}

}

void UBoopComponent::PushBall(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto ball = Cast<APlayBall>(OtherActor);
	if(!ball)
	{
		return;
	}
	
	auto cameraMngr = UGameplayStatics::GetPlayerCameraManager(Owner, UGameplayStatics::GetPlayerControllerID(PlayerController));

	FVector cameraForwardVec = cameraMngr->GetActorForwardVector();

	if (MovementComponent->GetCBMovementMode() == ECBMovementMode::CBMOVE_Running)
	{
		cameraForwardVec = cameraForwardVec.RotateAngleAxis(UpwardsAngle, FVector(1, 0, 0));
	}
	ball->PushBall(Force, cameraForwardVec);
}

void UBoopComponent::DeactivateBoopHitbox()
{
	//TODO: fire event
	BoopHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bBoopActive = false;
}

// Called every frame
void UBoopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//DEBUG
	if(bBoopActive)
		DrawDebugBox(Owner->GetWorld(), BoopHitbox->GetComponentLocation(), BoopHitbox->GetScaledBoxExtent(), BoopHitbox->GetComponentRotation().Quaternion(), FColor::Red, false, DeltaTime, 0, 3.f);

	{
		auto cameraMngr = UGameplayStatics::GetPlayerCameraManager(Owner, UGameplayStatics::GetPlayerControllerID(PlayerController));

		FVector cameraForwardVec = cameraMngr->GetActorForwardVector();

		//BoopHitbox->SetWorldRotation()
	}

	
	//these assignments do not work in BeginPlay(), because playercontroller is assigned afterwards at some point
	if(!PlayerController)
	{
		auto pawn = Cast<APawn>(Owner);
		if (!pawn)
		{
			return;
		}

		auto pc = Cast<APlayerController>(pawn->GetController());
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

