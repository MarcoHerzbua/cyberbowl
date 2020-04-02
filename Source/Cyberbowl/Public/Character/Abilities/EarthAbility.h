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

public:
	GENERATED_BODY()
	
	virtual void Fire() override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AEarthpillar> earthClass;

private:
	AEarthpillar* pillar;

	class AThirdPersonPlayerController* characterController;

	void SpawnPillar();

	class ACyberbowlCharacter* character;
};
