// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include "PlayBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBallBooped); // uwu

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
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayBall Properties")
	float MaxSpeed = 4000.f;

	UPROPERTY(BlueprintAssignable, Category = "BoopComponent")
	FOnBallBooped OnBallBooped;
		

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector StartPosition;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PushBall(float force, FVector direction);

	UFUNCTION(BlueprintCallable)
	void StopBall();

	UFUNCTION(BlueprintCallable)
	void ResumeBall();

	UFUNCTION(BlueprintCallable)
	void PlayBall();

	UFUNCTION(BlueprintCallable)
	void ResetBallPosition();
};
