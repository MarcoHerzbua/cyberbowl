// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerInput.h"
#include "WallrunComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERBOWL_API UWallrunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWallrunComponent();

protected:
	class UCBCharacterMovementComponent* MovementComponent;
	APlayerController* PlayerController;
	TArray<FInputActionKeyMapping> JumpKeyMapping;
	
	UPROPERTY(BlueprintReadOnly)
	USphereComponent* WallrunCollider;
	
	//UPROPERTY(BlueprintReadOnly)
	//int CountWallTouches = 0;
		
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckForWallrun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndWallrun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
