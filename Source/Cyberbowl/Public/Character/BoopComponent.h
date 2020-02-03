// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "BoopComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERBOWL_API UBoopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float Range = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float BoxWidth = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float BoxHeight = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boop Params")
	float Force = 5000.f;

	UBoopComponent();

protected:
	AActor* Owner;
	UInputComponent* InputComponent;
	APlayerController* PlayerController;
	
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void StartBoop();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
