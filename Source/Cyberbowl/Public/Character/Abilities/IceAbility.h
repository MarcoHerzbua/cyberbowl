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
public:
	virtual void Fire() override;

	void Targeting() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IceAbilityParams")
	float ConeLength = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IceAbilityParams")
	float ConeAngle = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IceAbilityParams")
	float FreezeDuration = 3.f;

	UPROPERTY()
	FTimerHandle FreezeTimerHandle;

	UPROPERTY()
	TArray<AActor*> FrozenActors;

	UFUNCTION(BlueprintCallable, Category = "IceAbility")
	void UnfreezeActors();
	
	bool IsWithinCone(FVector hitPoint, FVector coneDirectionNormal);
};
