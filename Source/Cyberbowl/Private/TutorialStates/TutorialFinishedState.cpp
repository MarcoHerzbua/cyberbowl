// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialStates/TutorialFinishedState.h"


#include "GameModesAndInstances/TutorialGameMode.h"
#include "Kismet/GameplayStatics.h"

void UTutorialFinishedState::EnterState()
{
	UKismetSystemLibrary::PrintString(this, "Entered TutorialFinishedState");
}

void UTutorialFinishedState::UpdateState(float deltaSeconds)
{
	stateTimePassed += deltaSeconds;
	if (stateTimePassed > 3.f)
	{
		Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(this))->AdvanceTutorial();
	}
}

void UTutorialFinishedState::ExitState()
{
	UGameplayStatics::OpenLevel(this, "MainMenuMap");
}
