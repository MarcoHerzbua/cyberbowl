// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "Character/Abilities/Earthpillar.h"
#include "EarthAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Abilities), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API UEarthAbility : public UAbilityBase
{
	GENERATED_BODY()
	
protected:

	void BeginPlay() override;
	
	void Fire() override;

	void Targeting() override;
	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void EndLeap();

	void DoLeap();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinTargetDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTargetDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetIndicatorRadius = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeapDuration = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeapHeight = 2000.f;

	UPROPERTY(BlueprintReadOnly)
	FVector LeapTarget;

	UPROPERTY(BlueprintReadOnly)
	FVector LeapStart;

	UPROPERTY()
	bool bValidTarget;

	UPROPERTY()
	FTimerHandle LeapTimerHandle;

	AEarthpillar* pillar;

	class AThirdPersonPlayerController* characterController;

	void SpawnPillar();

	class ACyberbowlCharacter* character;

public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AEarthpillar> earthClass;
};


