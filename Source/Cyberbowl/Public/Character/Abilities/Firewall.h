// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraSystem.h"
#include "Firewall.generated.h"

UCLASS()
class CYBERBOWL_API AFirewall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirewall();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//UStaticMesh* mesh;


public:	
	// Called every frame
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* box;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;
	virtual void Tick(float DeltaTime) override;
	int GetBoxHeight() { return box->GetUnscaledBoxExtent().Z; };
	//void SpawnEffect();
};
