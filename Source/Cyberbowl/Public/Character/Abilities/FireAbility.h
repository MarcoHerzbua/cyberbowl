// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/CyberbowlCharacter.h"
#include "FireAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Abilities), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API UFireAbility : public UAbilityBase
{
	
	GENERATED_BODY()
public:

	UFireAbility() = default;

	UPROPERTY(BlueprintReadOnly)
	UNiagaraComponent* fireComponent;
	
	UPROPERTY(BlueprintReadWrite)
	UNiagaraSystem* fireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float targetIndicatorRadius = 400.f;


private:

	virtual void BeginPlay() override;
	
	virtual void Fire() override;
	
	virtual void Targeting() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	ACyberbowlCharacter* character;
	UCameraComponent* camera;
	FVector fireWallPosition;
	bool validTarget;

};