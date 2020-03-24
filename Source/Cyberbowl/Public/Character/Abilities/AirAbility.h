// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AirAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Abilities), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API UAirAbility : public UAbilityBase
{
	GENERATED_BODY()

	virtual void Fire() override;

public:
	virtual void BeginPlay() override;

protected:

	UFUNCTION(BlueprintCallable)
	void ConvertMetersToUnrealUnits();
	
	UPROPERTY(BlueprintReadWrite)
	float radiusMeters;

	UPROPERTY(BlueprintReadWrite)
	float grabDurationSeconds;

	UPROPERTY()
	AActor* ball;

	UPROPERTY()
	USceneComponent* ballPulledAttachComponent;
	
};
