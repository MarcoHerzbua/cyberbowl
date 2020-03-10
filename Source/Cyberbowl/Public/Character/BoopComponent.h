// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "CBCharacterMovementComponent.h"
#include "BoopComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoopStarted);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERBOWL_API UBoopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBoopComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float Force = 5000.f;

	//Defines how much the ball is pushed upwards when the character is on the ground running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float UpwardsAngle = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float BoopDuration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float BoopCooldown = 1.f;

	UPROPERTY(BlueprintAssignable, Category = "BoopComponent")
	FOnBoopStarted OnBoopStarted;
	
protected:
	AActor* Owner;
	UInputComponent* InputComponent;
	APlayerController* PlayerController;
	UCBCharacterMovementComponent* MovementComponent;
	
	class UBoxComponent* BoopHitbox;
	FVector BoopHitboxInitialLocation;

	UPROPERTY()
	FTimerHandle BoopDurationHandle;

	UPROPERTY()
	FTimerHandle BoopCooldownHandle;

	UPROPERTY(BlueprintReadOnly)
	bool bBoopActive = false;
	UPROPERTY(BlueprintReadOnly)
	bool bBoopOnCooldown = false;
	
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void StartBoop();

	UFUNCTION()
	void PushBall(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DeactivateBoopHitbox();

	void OnBoopCooldown();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
