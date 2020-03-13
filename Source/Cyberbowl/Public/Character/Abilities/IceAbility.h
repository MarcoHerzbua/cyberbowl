// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "IceAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Abilities), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API UIceAbility : public UAbilityBase
{
	GENERATED_BODY()
	
	virtual void Fire() override;
};
