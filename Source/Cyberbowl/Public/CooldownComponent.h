// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CooldownComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERBOWL_API UCooldownComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCooldownComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetDashReady();

	UFUNCTION()
	void SetUltReady();

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle DashCooldownHandle;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle UltCooldownHandle;

	UPROPERTY(BlueprintReadWrite)
	float TotalDashCooldown;

	UPROPERTY(BlueprintReadWrite)
	bool DashReady = true;

	UPROPERTY(BlueprintReadWrite)
	float TotalUltCooldown;

	UPROPERTY(BlueprintReadWrite)
	bool UltReady = true;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartCooldown(FString ability);
};
