// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutorialStates/TutorialState.h"
#include "GoalScoringState.generated.h"

/**
 * 
 */
UCLASS()
class CYBERBOWL_API UGoalScoringState : public UTutorialState
{
	GENERATED_BODY()
	
public:
	void EnterState() override;
	void UpdateState(float deltaSeconds) override;
	void ExitState() override;

protected:
	float stateTimePassed;
};
