// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "WallrunComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERBOWL_API UWallrunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TimeToActivateWallrun = 0.2f;
	
	USphereComponent* WallrunCollider;
	UCharacterMovementComponent* MovementComponent;
	APlayerController* PlayerController;
	TArray<FInputActionKeyMapping> JumpKeyMapping;
	
	UWallrunComponent();

protected:
	UPROPERTY(BlueprintReadOnly)
	float JumpInputPressedDuration = 0.f;

	bool bOnWall = false;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckForWallrun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
