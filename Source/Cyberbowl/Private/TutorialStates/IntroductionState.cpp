// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialStates/IntroductionState.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameModesAndInstances/TutorialGameMode.h"
#include "Kismet/GameplayStatics.h"

void UIntroductionState::EnterState()
{
	UKismetSystemLibrary::PrintString(this, "Entered IntroductionState");
}

void UIntroductionState::UpdateState(float deltaSeconds)
{
	stateTimePassed += deltaSeconds;
	if (stateTimePassed > 3.f)
	{
		Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(this))->AdvanceTutorial();
	}
}

void UIntroductionState::ExitState()
{
	UKismetSystemLibrary::PrintString(this, "Exited GoalScoringState");
}
