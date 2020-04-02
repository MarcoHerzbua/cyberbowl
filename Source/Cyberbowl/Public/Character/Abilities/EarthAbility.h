// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "EarthAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Abilities), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API UEarthAbility : public UAbilityBase
{
	GENERATED_BODY()
	
protected:
	
	void Fire() override;

	void Targeting() override;
	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinTargetDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTargetDistance = 3000.f;

};
