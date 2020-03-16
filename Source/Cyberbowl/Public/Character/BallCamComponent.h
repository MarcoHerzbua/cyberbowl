// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallCamComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERBOWL_API UBallCamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallCamComponent();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void FocusBall(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void ToggleBallCam();

	UPROPERTY(BlueprintReadOnly)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* PlayBall;

	UPROPERTY(BlueprintReadOnly)
	bool bShouldFollowBall;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
