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

	OnActorHit.AddDynamic(this, &APlayBall::ResolveCollision);
}

void APlayBall::ResolveCollision(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FName otherCollisionProfile = Hit.Component->GetCollisionProfileName();
	float ballVelMagnitude = CachedVelocity.Size();

	//TODO: this ignores impact with ground; (23.4.) stadium is work in progress and arena collider is single mesh with profile "StadiumWall"
	//when stadium is done we need to add the proper profiles to all the meshes to the visual arena
	//(we might need to implement another raycast that ignores the arena collider)
	//and the hit result of the raycast should be used to determine the correct collision profile
	if(otherCollisionProfile == FName("StadiumWall") && Hit.ImpactNormal.Z != 1.f)
	{
		OnBallHit.Broadcast(otherCollisionProfile, ballVelMagnitude / MaxSpeed);
	}
}

// Called every frame
void APlayBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CachedVelocity = GetVelocity();
	if(CachedVelocity.Size() > MaxSpeed)
	{
		BallStaticMesh->SetAllPhysicsLinearVelocity(CachedVelocity.GetClampedToMaxSize(MaxSpeed)) ;
		CachedVelocity = GetVelocity();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *GetVelocity().ToString()));
}

void APlayBall::PushBall(float force, FVector direction)
{
	if(IsBallFrozen)
	{
		UnFreeze_Implementation();
	}

	//Is this only moving the mesh? -> Marco: the mesh is a scene component and the root of the actor, so everything in this actor is moved
	BallStaticMesh->AddImpulse(direction * force, NAME_None, true);
	
	OnBallBooped.Broadcast();
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
	if(IsBallFrozen)
	{
		return;
	}
	
	IsBallFrozen = true;
	BallStaticMesh->SetSimulatePhysics(false);
	OnBallFrozen.Broadcast();
}

void APlayBall::UnFreeze_Implementation()
{
	if (!IsBallFrozen)
	{
		return;
	}

	IsBallFrozen = false;
	BallStaticMesh->SetSimulatePhysics(true);
	OnBallUnfrozen.Broadcast();
}

void APlayBall::Launch_Implementation(FVector direction, float forceHorizontal, float forceVertical)
{
	FVector newVel = direction * forceHorizontal;
	newVel.Z = forceVertical;
	BallStaticMesh->AddImpulse(newVel, NAME_None, true);
}

