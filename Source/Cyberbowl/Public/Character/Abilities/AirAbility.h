// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AirAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrabModeExitByPush);

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

public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnGrabModeExitByPush OnGrabModeExitByPush;
	
protected:
	virtual void Fire() override;
	
	UFUNCTION(BlueprintCallable)
	void ConvertMetersToUnrealUnits();

	UFUNCTION()
	void ExitGrabModeByPush();

	UFUNCTION()
	void ExitGrabMode();

	UFUNCTION()
	void DestroyTornado();

	UPROPERTY(EditAnywhere)
	float grabRadiusMeters;

	UPROPERTY(EditAnywhere)
	float succSpeed;

	UPROPERTY(EditAnywhere)
	float grabDurationSeconds;

	UPROPERTY(EditAnywhere)
	float tornadoDuration;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* tornadoEffect;

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

	UPROPERTY(BlueprintReadOnly)
	UNiagaraComponent* tornadoComponent;
};
