// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include "Components/BoxComponent.h"
#include "Goal_Collider.generated.h"

UCLASS()
class CYBERBOWL_API AGoal_Collider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal_Collider();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Ball;
	

private:
	UPROPERTY(VisibleAnywhere)
	int Points;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	AInGameGameMode* CurrMode;
	
	UBoxComponent* BoxComponent;
};