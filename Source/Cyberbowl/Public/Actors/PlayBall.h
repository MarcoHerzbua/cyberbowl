// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PlayBall.generated.h"

UCLASS()
class CYBERBOWL_API APlayBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayBall();

	UStaticMeshComponent* BallStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayBall Properties")
	float ScaleModifier = 3.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PushBall(float force, FVector direction);
};
