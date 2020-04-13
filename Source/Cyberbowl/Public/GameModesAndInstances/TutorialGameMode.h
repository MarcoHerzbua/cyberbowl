// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PlayBall.h"
#include "GameFramework/GameModeBase.h"
#include "TutorialGameMode.generated.h"

class UTutorialState;

UCLASS()
class CYBERBOWL_API ATutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AdvanceTutorial();

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

protected:
	void SetupStates();
	
	TQueue<UTutorialState*> tutorialStates;

	UPROPERTY()
	UTutorialState* currentState;
};
