// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/TriggerBase.h"
#include "Earthpillar.generated.h"

UCLASS()
class CYBERBOWL_API AEarthpillar : public AActor
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties

	AEarthpillar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* cylinder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* triggerMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;

	UPROPERTY(EditAnywhere, Category = "EarthPillarParams")
	float MaxRise = 200.f;

	UPROPERTY(EditAnywhere, Category = "EarthPillarParams")
	float RiseTime = 10.f;

	UPROPERTY(EditAnywhere, Category = "EarthPillarParams")
	float LoweringTime = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "EarthPillarParams")
	float LaunchForceVertical = 8000.f;
	
	UPROPERTY(EditAnywhere, Category = "EarthPillarParams")
	float LaunchForceHorizontal = 12000.f;
	
	UPROPERTY(EditAnywhere, Category = "EarthPillarParams")
	float LaunchCooldown = 0.2f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//void SetCurrPlayerTeam(int playerTeam);

	//void SetMaxLoweringPos(float pos);
	

	UFUNCTION(BlueprintCallable)
	void InitializePillar(int playerTeam, float maxLoweringPos, float lifeSpan);
	
	float GetPillarLocationZ();
	
protected:
	FRotator rotation;

	void Rising(float DeltaTime);
	void Lowering(float DeltaTime);

	bool bIsRising;
	bool bIsLowering;
	int currPlayerTeam;
	float maxLowering;

	UPROPERTY()
	FTimerHandle LaunchTimerHandle;

	//UPROPERTY(BlueprintReadOnly)
	//AActor* LaunchedActor;

	//UPROPERTY(BlueprintReadOnly)
	//FVector LaunchTarget;

	//UPROPERTY(BlueprintReadOnly)
	//FVector LaunchStart;

	//UPROPERTY(BlueprintReadOnly)
	//float LaunchDuration;

	//UPROPERTY(BlueprintReadOnly)
	//float LaunchHeight;

	
	void TickLaunch();

	void EndLaunch();
};
