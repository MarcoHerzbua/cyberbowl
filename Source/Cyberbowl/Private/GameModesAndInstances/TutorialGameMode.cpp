// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModesAndInstances/TutorialGameMode.h"
#include "TutorialStates/GoalScoringState.h"
#include "TutorialStates/TutorialState.h"
#include "TutorialStates/IntroductionState.h"
#include "TutorialStates/TutorialFinishedState.h"

void ATutorialGameMode::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	
	SetupStates();

	tutorialStates.Dequeue(currentState);
	currentState->EnterState();
}

void ATutorialGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	currentState->UpdateState(DeltaSeconds);
}

void ATutorialGameMode::SetupStates()
{
	tutorialStates.Enqueue(NewObject<UIntroductionState>(GetWorld()));
	tutorialStates.Enqueue(NewObject<UGoalScoringState>(GetWorld()));
	tutorialStates.Enqueue(NewObject<UTutorialFinishedState>(GetWorld()));
}

void ATutorialGameMode::AdvanceTutorial()
{
	currentState->ExitState();
	
	if (tutorialStates.IsEmpty())
	{
		return;
	}
	
	tutorialStates.Dequeue(currentState);
	currentState->EnterState();
}
