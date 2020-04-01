// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AirAbility.generated.h"

class APlayBall;
class UCharacterMovementComponent;
class ACyberbowlCharacter;
class USpringArmComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(ClassGroup = (Abilities), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API UAirAbility : public UAbilityBase
{
	GENERATED_BODY()

	

	virtual void Fire() override;

public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:

	UFUNCTION(BlueprintCallable)
	void ConvertMetersToUnrealUnits();

	UFUNCTION()
	void ExitGrabMode();

	UFUNCTION()
	void DestroyTornado();
	
	UPROPERTY(BlueprintReadWrite)
	float grabRadiusMeters;

	UPROPERTY(BlueprintReadWrite)
	float succSpeed;

	UPROPERTY(BlueprintReadWrite)
	float grabDurationSeconds;

	UPROPERTY(BlueprintReadWrite)
	float tornadoDuration;
	
	UPROPERTY()
	APlayBall* ball;

	UPROPERTY()
	USceneComponent* ballPulledAttachComponent;

	UPROPERTY()
	USpringArmComponent* ballLocationSpringArm;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInGrabMode;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle GrabModeDurationHandle;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TornadoEffectDurationHandle;

	UPROPERTY()
	UCharacterMovementComponent* movementComp;

	UPROPERTY()
	ACyberbowlCharacter* character;

	UPROPERTY(BlueprintReadWrite)
	UNiagaraSystem* tornadoEffect;

	UPROPERTY(BlueprintReadOnly)
	UNiagaraComponent* tornadoComponent;

	
};
